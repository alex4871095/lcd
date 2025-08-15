#include <stdio.h>

int main() {
    void *ptr = (void *)0x123456789ABCDEF0;  // 64-битный указатель (пример адреса)
    
    int x = (int)ptr;  // Приведение указателя к int — **потеря старших 32 бит**
    
    void *ptr_copy = (void *)x;  // Восстановление из обрезанного значения
    
    printf("Исходный указатель : %p\n", ptr);
    printf("После приведения к int : 0x%x\n", x);
    printf("Восстановленный указатель : %p\n", ptr_copy);

    // Доступ к ptr_copy ведет к ошибкам, т.к. адрес неверный
    // printf("%p\n", *(int **)ptr_copy); // потенциально опасно

    return 0;
}

