#include <stdio.h>

#define FIFO_NAME "./fifofile"

int main ()
{
  FILE * f;
  char ch;
  const char resp[2] = "ok";
  f = fopen(FIFO_NAME, "r");
  do
  {
    ch = fgetc(f);
    putchar(ch);
  } while (ch != 'q');
  fclose(f);
  f = fopen(FIFO_NAME, "w");
  fputs(resp, f);
  fclose(f);
  unlink(FIFO_NAME);
  return 0;
}
