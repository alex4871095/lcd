#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char ** argv) {
  struct sigaction sa;
  sigset_t newset;
  pid_t pid;
  int signal;

  if (argc < 3) 
    {
       fprintf(stderr,"usage: %s <pid> <signal>\n", argv[0]);
       return EXIT_FAILURE;
    }

  pid = atoi(argv[1]);
  signal = atoi(argv[2]);

  printf("Sending signal %d to process %d\n", signal, pid);
  kill(pid, signal);

  return 0;
}
