#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME "./fifofile" 

int main(int argc, char * argv[])
{
  FILE * f;
  char ch;
  char resp[2];
  mkfifo(FIFO_NAME, 0600); 
  f = fopen(FIFO_NAME, "w");
  if (f == NULL) 
  { 
    printf("Не удалось открыть файл\n");
    return -1;
  }
  do
  {
    ch = getchar();
    fputc(ch, f);
    if (ch == 10) fflush(f);
  } while (ch != 'q');
  fclose(f);
  f = fopen(FIFO_NAME, "r");
  fgets(resp, 3, f);
  fclose(f);
  printf(resp);
  unlink(FIFO_NAME);
  return 0;
}
