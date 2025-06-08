#include <stdio.h>
#include <stdlib.h>

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

