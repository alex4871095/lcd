#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#define ODD 1
#define EVEN 0
#define SIZE 10000

struct thread_arg {
  int *arr;
  int size;
  int type;
  long sum;
};

void * thread_func(void *arg)
{
  int i;
  struct thread_arg *args = (struct thread_arg *)arg;
  
  if(args->type == 1) {
    for(i = 1; i < args->size; i +=2) 
      args->sum =+args->arr[i]; 
  }
  else {
    for(i = 0; i < args->size; i +=2) 
      args->sum =+args->arr[i];
  }
}

int main(int argc, char * argv[])
{
  struct thread_arg arg[2];
  int i, result;
  int arr[SIZE];
  pthread_t thread1, thread2;

  srand(time(0));
  for(i=0; i < SIZE; i++)
    arr[i] = rand();

  arg[0].arr = arr;
  arg[0].size = SIZE;
  arg[0].type = ODD;
  arg[0].sum = 0;
  result = pthread_create(&thread1, NULL, thread_func, &arg[0]);
  if (result != 0) {
    perror("Creating the first thread");
    return EXIT_FAILURE;
  }

  arg[1].arr = arr;
  arg[1].size = SIZE;
  arg[1].type = EVEN;
  arg[1].sum = 0;
  result = pthread_create(&thread2, NULL, thread_func, &arg[1]);
  if (result != 0) {
    perror("Creating the second thread");
    return EXIT_FAILURE;
  }

  result = pthread_join(thread1, NULL);
  if (result != 0) {
    perror("Joining the first thread");
    return EXIT_FAILURE;
  }
  printf("Sum returned by thread1 for odd numbers is %lu\n", arg[0].sum);

  result = pthread_join(thread2, NULL);
  if (result != 0) {
    perror("Joining the second thread");
    return EXIT_FAILURE;
  }
  printf("Sum returned by thread2 for even numbers is %lu\n", arg[1].sum);

  printf("Final sum is %lu\n", arg[0].sum+arg[1].sum);
  return EXIT_SUCCESS;
}
