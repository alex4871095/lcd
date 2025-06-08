#include <stdio.h>
#include <stdlib.h>

void swap(int* array, int i, int j) {
    int tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}

void array_sort(int array[], int n) {
    for (int i = 0; i < n - 1; i++) {
      
        for (int j = 0; j < n - i - 1; j++) {
            if (array[j] > array[j + 1])
                swap(array, j, j + 1);
        }
    }
}

