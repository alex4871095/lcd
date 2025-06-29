#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;

void *functionCount(void *arg);
int count = 0;
int order = 1;
int COUNT_DONE = 10;

int main()
{
    pthread_t thread1, thread2, thread3;
    int id1 = 1, id2 = 2, id3 = 3;

    pthread_create( &thread1, NULL, &functionCount, &id1);
    pthread_create( &thread2, NULL, &functionCount, &id2);
    pthread_create( &thread3, NULL, &functionCount, &id3);

    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL);
    pthread_join( thread3, NULL);

    printf("Count is finished\n");
    exit(0);
}

void *functionCount(void *arg)
{
    int i = *((int*)arg);

    while(1)
    {
      if(count >= COUNT_DONE) return(NULL);

      if(i == order)
      {
        pthread_mutex_lock( &count_mutex );
        count++;
        printf("This is thread %d working, counter is %d\n", i, count);
        if(order == 3) {
          order = 1;
        }
        else {
          order++;
        }
        pthread_mutex_unlock( &count_mutex );
      }
    }
}

