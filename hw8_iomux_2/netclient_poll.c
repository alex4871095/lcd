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
#include <pthread.h>

#define BUF_SIZE 80

int send_flag = 0;
char send_buffer[BUF_SIZE];

void* blocking_io(void *arg)
{
  do
  {
    printf("Enter message:\n");
    memset(send_buffer, 0, BUF_SIZE);
    fgets(send_buffer, BUF_SIZE-1, stdin);
    send_flag = 1;
    sleep(1);
  } while(1);
}

int main (int argc, char **argv)
{
  int port, rc, pid, timeout, s, on = 1;
  int new_sd = -1;
  char recv_buffer[BUF_SIZE];
  struct sockaddr_in6 serv_addr;
  struct addrinfo hints, *result, *rp; 
  struct pollfd fds;
  pthread_t thread_io;

  if (argc < 3) 
  {
     fprintf(stderr,"usage: %s <hostname> <port_number>\n", argv[0]);
     return EXIT_FAILURE;
  }

  port = atoi(argv[2]);

  new_sd = socket(AF_INET6, SOCK_STREAM, 0);
  if (new_sd < 0)
  {
    printf("socket() failed: %d", errno);
    return EXIT_FAILURE;
  }

  rc = fcntl(new_sd, F_SETFL, O_NONBLOCK);

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
    rc = connect(new_sd, rp->ai_addr, rp->ai_addrlen);
    if(rc < 0)
    {
      continue;
    }
    else
      break;
  }

  printf("Connected to remote server\n");

  pthread_create(&thread_io, NULL, blocking_io, NULL);

  printf("Thread started for io\n");

  memset(&fds, 0 , sizeof(fds));
  fds.fd = new_sd;
  fds.events = POLLIN | POLLOUT;

  do
  {
    rc = poll(&fds, 1, 1000);

    if (rc < 0)
    {
      perror("poll() failed");
      break;
    }

    if(send_flag == 1)
    {
      printf("Sending message..\n");
      rc = send(fds.fd, send_buffer, sizeof(send_buffer), 0);

      if(rc < 0 && errno != EWOULDBLOCK)
        printf("Error sending, errno = %d\n", errno);
      else
      {
        printf("EWOULDBLOCK errno..\n");
        printf("After EWOULDBLOCK we will wait for POLLOUT, need to track how much we've send and keep the rest for next time..\n");
        printf("Hope that will be implemented later..\n");
      }
      send_flag = 0;
    }

    if(fds.revents == 0)
    {
      continue;
    }
    else
    {
      if(!(fds.revents & POLLIN) && !(fds.revents & POLLOUT))
      {
        printf("Error! revents = %d\n", fds.revents);
        break;
      }

      if(fds.revents & POLLIN)
      {
        printf("Receiving message..\n");
        rc = recv(fds.fd, recv_buffer, sizeof(recv_buffer), 0);
        if (rc < 0)
        {
          if (errno != EWOULDBLOCK)
          {
            perror("recv() failed");
          }
          break;
        }    

        if (rc == 0)
        {
          printf(" Connection closed\n");
          break;
        }

        printf("Msg from server: %s\n", recv_buffer);
      }

      if(fds.revents & POLLOUT)
      {
        if(send_flag == 1)
        {
          printf("Sending message..\n");
          rc = send(fds.fd, send_buffer, sizeof(send_buffer), 0);

          if(rc < 0 && errno != EWOULDBLOCK)
            printf("Error sending, errno = %d\n", errno);

          send_flag = 0;
        }
      }
    }
    sleep(1);
  } while(1);

  close(new_sd);
}

