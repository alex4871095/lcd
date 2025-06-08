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
#include <fcntl.h>

#define SERVER_PORT 12345
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

//    printf("Sending message..\n");
//    rc = send(sock, buffer, sizeof(buffer), 0);

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
  int rc, pid, timeout, is_done = 0, on = 1;
  int listen_sd = -1, new_sd = -1;
  char buffer[80];
  struct sockaddr_in6 addr;
  struct pollfd fds;

  listen_sd = socket(AF_INET6, SOCK_STREAM, 0);
  if (listen_sd < 0)
  {
    perror("socket() failed");
    exit(-1);
  }

  rc = setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
  if (rc < 0)
  {
    perror("setsockopt() failed");
    close(listen_sd);
    exit(-1);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sin6_family = AF_INET6;
  memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
  addr.sin6_port = htons(SERVER_PORT);

  rc = bind(listen_sd, (struct sockaddr *)&addr, sizeof(addr));
  if (rc < 0)
  {
    perror("bind() failed");
    close(listen_sd);
    exit(-1);
  }

  rc = listen(listen_sd, 32);
  if (rc < 0)
  {
    perror("listen() failed");
    close(listen_sd);
    exit(-1);
  }

  new_sd = accept(listen_sd, NULL, NULL);

  if (new_sd < 0)
  {
    perror(" accept() failed");
    exit(1);
  }

  printf("Accepted new connection\n");
  
  rc = fcntl(new_sd, F_SETFL, O_NONBLOCK);

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

      printf("Receiving message..\n");
      rc = recv(fds.fd, buffer, sizeof(buffer), 0);
      if (rc < 0)
      {
        if (errno != EWOULDBLOCK)
        {
          perror(" recv() failed");
        }
        break;
      }    

      if (rc == 0)
      {
        printf(" Connection closed\n");
        break;
      }

      printf("Msg from client: %s\n", buffer);

      if(is_done == 0)
      {
        pid = fork();
        if(pid == 0)
          blocking_send(new_sd);
        is_done = 1;
      }
    }
  } while(1);

  close(new_sd);
  close(listen_sd);

}
