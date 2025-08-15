#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    const char *filename = "test.dat";
    int fd = open(filename, O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    size_t filesize = 4096; // 4KB

    // Устанавливаем размер файла
    if (ftruncate(fd, filesize) == -1) {
        perror("ftruncate");
        close(fd);
        return 1;
    }

    // Отображаем файл в память с правами чтения и записи
    char *data = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    // Записываем строку в память (а значит, и в файл)
    const char *text = "Hello, world!";
    memcpy(data, text, strlen(text));

    // Синхронизация данных с файлом (не обязательно, ОС сделает позже сама)
    if (msync(data, filesize, MS_SYNC) == -1) {
        perror("msync");
    }

    munmap(data, filesize);
    close(fd);
    return 0;
}

