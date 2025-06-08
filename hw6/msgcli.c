#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <strings.h>
#include "msgtypes.h"
#include <string.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
  int msgid;
  int i;
  struct msg_t message;
  msgid = msgget(KEY, 0666);
  if (msgid == -1) 
  {
     printf("Server is not running!\n", msgid);
     return EXIT_FAILURE;
  }

  do { 
    do {
      i = 0;
      while ( (i < (MAXLEN - 1)) && ((message.body[i++] = getchar()) !=  '\n') );
      message.body[i] = '\0';
      message.mtype = 2;
      message.snd_pid = getpid ();
      msgsnd(msgid, &message, sizeof(message) - sizeof(message.mtype), 0);
    } while(strncmp(message.body, "listen", 6) != 0 && strncmp(message.body, "exit", 4) != 0);

    if(strncmp(message.body, "exit", 4) == 0) break;
    do { 
      msgrcv(msgid, &message, sizeof(message) - sizeof(message.mtype), 1, 0);
      printf("Server (pid = %i) sent: %s", message.snd_pid, message.body);
    } while(strncmp(message.body, "listen", 6) != 0 && strncmp(message.body, "exit", 4) != 0);

  } while(strncmp(message.body, "exit", 4) != 0);

  return EXIT_SUCCESS;
}
