#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>

#define BUF_SIZE 80

void blocking_send(int sock)
{
  int rc;
  char buffer[BUF_SIZE];
  struct pollfd fds;

  do
  {
    printf("Enter message:\n");
    memset(buffer, 0, BUF_SIZE);
    fgets(buffer, BUF_SIZE-1, stdin);

    printf("Sending message..\n");
    rc = send(sock, buffer, sizeof(buffer), 0);

    if(errno == EWOULDBLOCK)
      printf("EWOULDBLOCK errno..\n");

    if(rc < 0)
      printf("Error sending, errno = %d\n", errno);

    memset(&fds, 0, sizeof(fds));
    fds.fd = sock;
    fds.events = POLLOUT;

    do
    {
      printf("Begin polling for pollout..\n");
      rc = poll(&fds, 1, 1000);

      if (rc < 0)
      {
        perror(" poll() failed");
        break;
      }

      if(fds.revents == 0)
        continue;
      else
      {
        if(fds.revents != POLLOUT)
        {
          printf(" Error! revents = %d\n", fds.revents);
          break;
        }

        printf("Sending message..\n");
        rc = send(fds.fd, buffer, sizeof(buffer), 0);
        if (rc < 0)
        {
          perror(" send() failed");
          break;
        }
        else
          break;
      }
    } while(1);
  } while(1);
}


int main (int argc, char **argv)
{
  int sock, port, rc, pid, timeout, s, is_done = 0, on = 1;
  int new_sd = -1;
  char buffer[80];
  struct sockaddr_in6 serv_addr;
  struct addrinfo hints, *result, *rp; 
  struct pollfd fds;

  if (argc < 3) 
  {
     fprintf(stderr,"usage: %s <hostname> <port_number>\n", argv[0]);
     return EXIT_FAILURE;
  }

  port = atoi(argv[2]);

  sock = socket(AF_INET6, SOCK_STREAM, 0);
  if (sock < 0)
  {
    printf("socket() failed: %d", errno);
    return EXIT_FAILURE;
  }

  rc = fcntl(sock, F_SETFL, O_NONBLOCK);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_protocol = 0;

  s = getaddrinfo(argv[1], argv[2], &hints, &result);

  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  for (rp = result; rp != NULL; rp = rp->ai_next)
  {
    new_sd = connect(sock, rp->ai_addr, rp->ai_addrlen);
    if(new_sd < 0)
    {
      continue;
    }
    else
      break;
  }

  rc = fcntl(new_sd, F_SETFL, O_NONBLOCK);

  printf("Connected to remote server\n");

  pid = fork();
  if(pid == 0)
    blocking_send(new_sd);

  printf("Forked for sending, continue..\n");

  memset(&fds, 0 , sizeof(fds));
  fds.fd = new_sd;
  fds.events = POLLIN;

  do
  {
    printf("Begin polling for pollin..\n");
    rc = poll(&fds, 1, 1000);

    if (rc < 0)
    {
      perror(" poll() failed");
      break;
    }

    if(fds.revents == 0)
      continue;
    else
    {
      if(fds.revents != POLLIN)
      {
        printf(" Error! revents = %d\n", fds.revents);
        break;
      }

      rc = recv(fds.fd, buffer, sizeof(buffer), 0);
      if (rc < 0)
      {
        perror(" recv() failed");
        break;
      }    

      if (rc == 0)
      {
        printf(" Connection closed\n");
        break;
      }

      printf("Msg from server: %s\n", buffer);
    }
  } while(1);

  close(new_sd);
}
