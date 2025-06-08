#include <stdio.h>
#include <dirent.h>
#include <string.h>


int sel(struct dirent * d)
{
  if(strstr(d->d_name, ".c")) return 1;
  return 0;
}

int main (int argc, char ** argv) {
  int i, n;
  struct dirent ** entry;
  if (argc != 2) {
    printf("Использование: %s <директория>\n", argv[0]);
    return 0;
  }
  n = scandir(argv[1], &entry, sel, alphasort);
  if (n < 0) 
  {
     printf("Ошибка чтения каталога\n");
     return 1;
  }
  for (i = 0; i < n; i++) 
    printf("%s inode=%i\n", entry[i]->d_name, entry[i]->d_ino);
  return 0;
}
