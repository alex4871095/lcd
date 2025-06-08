#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     /* read(), write(), close() */
#include <fcntl.h>      /* open(), O_RDONLY */
#include <sys/stat.h>   /* S_IRUSR */
#include <sys/types.h>  /* mode_t */

#define FN "array"

void array_fill(int array[], int n)
{
	int i;
	for(i=0; i<n; i++)
		array[i] = rand(); 
}

void array_output(int array[], int n)
{
	int i;
	for(i=0; i<n; i++)
		printf("%d ", array[i]);
	printf("\n");
}

int array_write(int array[], int n)
{
	int i, fd, wb;
	void *buffer = malloc(n * sizeof(int));
	mode_t mode = S_IRUSR | S_IWUSR;
        int flags = O_WRONLY | O_CREAT; 

	fd = open(FN, flags, mode);
        if (fd < 0)
        {
                fprintf (stderr, "Cannot open file\n");
                exit (1);
        }       
	buffer = array;
	wb = write(fd, buffer, n * sizeof(int));
	close(fd);
	return wb;
}

int array_read(int array[], int n)
{
	int i, fd, rb;
	void *buffer = malloc(n * sizeof(int));
        mode_t mode = S_IRUSR | S_IWUSR;
        int flags = O_RDONLY;

        fd = open(FN, flags, mode);
        if (fd < 0)
        {
                fprintf (stderr, "Cannot open file\n");
                exit (1);
        }
        rb = read(fd, buffer, n * sizeof(int));
	array = buffer;
        close(fd);
        return rb;
}
