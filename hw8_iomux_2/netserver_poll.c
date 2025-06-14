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
#include <pthread.h>

#define SERVER_PORT 12345
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
  int rc, pid, timeout, on = 1;
  int listen_sd = -1, new_sd = -1;
  char recv_buffer[BUF_SIZE];
  struct sockaddr_in6 addr;
  struct pollfd fds;
  pthread_t thread_io;

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
    perror("accept() failed");
    exit(-1);
  }

  printf("Accepted new connection\n");
  
  rc = fcntl(new_sd, F_SETFL, O_NONBLOCK);

  if (rc < 0)
  {
    perror("fcntl() failed");
    close(listen_sd);
    close(new_sd);
    exit(-1);
  }

  pthread_create(&thread_io, NULL, blocking_io, NULL);

  printf("Thread started for io..\n");

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

      if(rc < 0)
      {
        if (errno == EWOULDBLOCK)
        { 
          printf("errno = EWOULDBLOCK..\n");
        }
        else
          printf("Error sending, errno = %d\n", errno);
      }
      else
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

        printf("Msg from client: %s\n", recv_buffer);
      }

      if(fds.revents & POLLOUT)
      {
        if(send_flag == 1)
        {
          printf("Sending message..\n");
          rc = send(fds.fd, send_buffer, sizeof(send_buffer), 0);

          if(rc < 0)
          {
            if (errno == EWOULDBLOCK)
            {
              printf("errno = EWOULDBLOCK..\n");
            }
            else
              printf("Error sending, errno = %d\n", errno);
          }
          else
            send_flag = 0;
        }
      }
    }
    sleep(1);
  } while((strncmp(send_buffer, "quit", 4) != 0) && (strncmp(recv_buffer, "quit", 4) != 0));

  close(new_sd);
  close(listen_sd);
}
