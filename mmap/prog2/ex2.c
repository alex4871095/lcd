#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    size_t size = 4096; // Размер выделяемой области — 1 страница

    // Создаём анонимное отображение памяти с правами на чтение и запись
    void *area = mmap(NULL, size, PROT_READ | PROT_WRITE,
                      MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (area == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // Записываем строку в выделенную область памяти
    strcpy((char *)area, "Hello anonymous mmap!\0");

    // Читаем и выводим содержимое
    printf("%s\n", (char *)area);

    // Опционально: задержка для возможности исследовать процесс
    sleep(10);

    // Освобождаем память
    munmap(area, size);
    return 0;
}

