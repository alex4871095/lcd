#include <stdio.h>
#include <stdint.h>

int main() {
    void *ptr = (void *)0x123456789ABCDEF0;  // 64-битный указатель (пример адреса)
    
    uintptr_t x = (uintptr_t)ptr;  // Приведение указателя к uintptr_t (беззнаковый целочисленный тип подходящего размера)
    
    void *ptr_copy = (void *)x;  // Восстановление указателя из целочисленного значения
    
    printf("Исходный указатель : %p\n", ptr);
    printf("После приведения к uintptr_t : 0x%lx\n", (unsigned long)x);
    printf("Восстановленный указатель : %p\n", ptr_copy);

    // Безопасное использование указателя ptr_copy

    return 0;
}

