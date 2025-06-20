#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

void term_handler(int sig) {
  printf("Signal %i - %s\n", sig, strsignal(sig));
  exit(EXIT_SUCCESS);
}

int main(int argc, char ** argv) {
  sigset_t sset;
  int sig;
  struct sigaction sa;
  sa.sa_handler = term_handler;
  sigaction(SIGTERM, &sa, 0);
  sigfillset(&sset);
  sigdelset(&sset, SIGTERM);
  sigprocmask(SIG_SETMASK, &sset, 0);
  printf("My pid is %i\n", getpid());
  while(!sigwait(&sset, &sig))
    printf("Signal %i - %s\n", sig, strsignal(sig));
    //printf("%s\n", strsignal(sig));
  return EXIT_SUCCESS;
}
