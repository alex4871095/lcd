#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void term_handler(int i) {
  printf ("Got signal in child, terminating\n");
  exit(EXIT_SUCCESS);
}

void child_func(void) {
  struct sigaction sa;

  sa.sa_handler = term_handler;
  sigaction(SIGTERM, &sa, 0);

  printf("I'm child with pid %d\n", getpid());
  while(1) sleep(1);
}

int main(int argc, char ** argv) {
  pid_t pid;

  printf("My pid is %i\n", getpid());
  printf("Forking...\n");
  pid = fork();
  if (pid == 0)
  {
    child_func();
  }
  else
  {
    sleep(1);
    printf("I'm parent, sending signal 15 to child with pid %d\n", pid);
    kill(pid, 15);
  }

  return 0;
}
