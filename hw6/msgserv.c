#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "msgtypes.h"
#include <unistd.h>

int main(int argc, char * argv[])
{
  struct msg_t message;
  int msgid;
  int i;
  msgid = msgget(KEY, 0666 | IPC_CREAT);

  do {
    do {
      msgrcv(msgid, &message, sizeof(message) - sizeof(message.mtype), 2, 0);
      printf("Client (pid = %i) sent: %s", message.snd_pid, message.body);
    } while(strncmp(message.body, "listen", 6) != 0 && strncmp(message.body, "exit", 4) != 0);

    if(strncmp(message.body, "exit", 4) == 0) break;
    do { 
      i = 0;
      while ( (i < (MAXLEN - 1)) && ((message.body[i++] = getchar()) !=  '\n') );
      message.body[i] = '\0';
      message.mtype = 1;
      message.snd_pid = getpid ();
      msgsnd(msgid, &message, sizeof(message) - sizeof(message.mtype), 0);
    } while(strncmp(message.body, "listen", 6) != 0 && strncmp(message.body, "exit", 4) != 0);

  } while(strncmp(message.body, "exit", 4) != 0);

  msgctl(msgid, IPC_RMID, 0);
  return EXIT_SUCCESS;
}
