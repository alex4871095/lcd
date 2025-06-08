#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shmemtypes.h"
#include <time.h>

int main(int argc, char * argv[])
{
  key_t key;
  int shmid;
  struct memory_block * mblock;
  int i;
  key = ftok(FTOK_FILE, 1); // генерация ключа
  if (key == -1)
  {
     printf("Failed to generate unique key. Server compiler with a wrong name?\n");
     return EXIT_FAILURE;
  }
/* выделяем блок разделяемой памяти */
  shmid = shmget(key, sizeof(struct memory_block), 0666 | IPC_CREAT); 
/* отображаем блок разделяемой памяти в адресное пространство процесса */
  mblock = (struct memory_block *) shmat(shmid, 0, 0); 
  mblock->turn = CLIENT;
  mblock->server_lock = FREE;
  mblock->client_lock = FREE;
  mblock->readlast = SERVER;
  printf("Press any key to generate array, 'q' for exit\n");
  while (getchar() != 'q')
  {
//     mblock->server_lock = BUSY;
//     mblock->turn = CLIENT;
     while ((mblock->client_lock == BUSY) && (mblock->turn == CLIENT));
     mblock->server_lock = BUSY;
     mblock->turn = CLIENT;
     if (mblock->readlast == CLIENT)
     {
       mblock->readlast = SERVER;
       for(i = 0; i < MAXLEN; i++)
         mblock->array[i] = rand() % (1000 - 1 +1) + 1;
       mblock->server_lock = FREE;
     }
  }
  printf("Server got q and exits\n");
  shmdt((void *) mblock);  // удаляем отображение
  shmctl(shmid, IPC_RMID, 0);     // удаляем блок разделяемой памяти
  return EXIT_SUCCESS;
} 
