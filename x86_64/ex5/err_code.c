#include <stdio.h>

int main() {
    int array[100];
    int *ptr_start = &array[0];
    int *ptr_end = &array[50];

    // Ошибка: разность указателей сохраняется в int, что может быть некорректно в 64-битной системе
    int diff = ptr_end - ptr_start;

    printf("Difference (int): %d\n", diff);

    // Адресная арифметика выполняется с 32-битным типом, что может привести к ошибкам при больших размерах
    int offset = diff + 1000000000;  // может привести к переполнению
    printf("Offset with int: %d\n", offset);

    return 0;
}

