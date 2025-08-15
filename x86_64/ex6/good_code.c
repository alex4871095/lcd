#include <stdio.h>
#include <stdint.h> // Для uintptr_t

int main() {
    unsigned int shift_amount = 35;
    uint64_t value = 1;   // 64-битный тип для правильного сдвига

    // Правильный сдвиг 64-битного значения на 35 бит
    uint64_t result = value << shift_amount;

    // Адресная арифметика с uintptr_t — безопасная работа с указателями
    void *ptr = (void *)0x123456789ABCDEF0;    // Пример 64-битного указателя
    uintptr_t int_ptr = (uintptr_t)ptr;        // Безопасное хранение адреса
    uintptr_t shifted_ptr = int_ptr << 2;      // Сдвиг 64-битного целого — корректно

    printf("Результат сдвига value << %u = 0x%lx\n", shift_amount, result);
    printf("Сдвинутый указатель (uintptr_t) = 0x%lx\n", shifted_ptr);

    return 0;
}

