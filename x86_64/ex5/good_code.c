#include <stdio.h>
#include <stddef.h>    // для ptrdiff_t
#include <stdint.h>    // для uintptr_t

int main() {
    int array[100];
    int *ptr_start = &array[0];
    int *ptr_end = &array[50];

    // Правильное использование ptrdiff_t для хранения результата вычитания указателей
    ptrdiff_t diff = ptr_end - ptr_start;

    printf("Difference (ptrdiff_t): %td\n", diff);

    // Используем uintptr_t для адресной арифметики, например, для вычисления смещения адреса
    uintptr_t addr_start = (uintptr_t)ptr_start;
    uintptr_t addr_offset = addr_start + diff * sizeof(int);

    printf("Address start: %p\n", ptr_start);
    printf("Address offset: %p\n", (void *)addr_offset);

    return 0;
}

