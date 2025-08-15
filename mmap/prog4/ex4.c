#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

// Машинный код, который вызывает write(1, msg, 13), потом exit(0).
// msg встроена прямо после кода.
// Используется RIP-relative адресация: lea rsi, [rip + offset_to_msg]
// Смещение в инструкции прописано так, чтобы попасть на строку ниже по памяти.
unsigned char code[] = {
    0x48, 0xc7, 0xc0, 0x01,0x00,0x00,0x00,          // mov rax, 1
    0x48, 0xc7, 0xc7, 0x01,0x00,0x00,0x00,          // mov rdi, 1
    0x48, 0x8d, 0x35, 0x15,0x00,0x00,0x00,          // lea rsi, [rip+0x15]
    0x48, 0xc7, 0xc2, 0x0d,0x00,0x00,0x00,          // mov rdx, 13
    0x0f, 0x05,                                      // syscall
    0x48, 0xc7, 0xc0, 0x3c,0x00,0x00,0x00,          // mov rax, 60
    0x48, 0x31, 0xff,                                // xor rdi, rdi
    0x0f, 0x05,                                      // syscall
    // "Hello, world\n"
    0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x2c, 0x20, 0x77,
    0x6f, 0x72, 0x6c, 0x64, 0x0a
};

int main() {
    size_t size = 4096;

    void* mem = mmap(NULL, size, PROT_READ | PROT_WRITE | PROT_EXEC,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(mem == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // Копируем код вместе с строкой
    memcpy(mem, code, sizeof(code));

    // Определяем функцию
    void (*func)() = mem;

    // Запускаем функцию, она напечатает "Hello, world\n" и завершится
    func();

    munmap(mem, size);
    return 0; // сюда не дойдёт, exit syscall завершит программу
}

