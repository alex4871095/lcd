#include <stdio.h>
#include <stdint.h>

int main() {
    unsigned int shift_amount = 35; // Сдвиг на 35 бит (больше, чем размер int = 32)
    unsigned int value = 1;

    // Ошибка: сдвигаем 32-битный int на 35 бит (неопределённое поведение)
    unsigned int result = value << shift_amount;

    // Адресная арифметика — приведение указателя к int (32 бит) и сдвиг
    void *ptr = (void *)0x123456789ABCDEF0; // 64-битный указатель (пример)
    int int_ptr = (int)(uintptr_t)ptr;      // Потеря старших бит адреса
    int_ptr = int_ptr << 2;                  // Сдвиг 32-битного int — может привести к ошибке

    printf("Результат сдвига value << %u = 0x%x\n", shift_amount, result);
    printf("Потерянный указатель после сдвига (int_ptr) = 0x%x\n", int_ptr);

    return 0;
}

