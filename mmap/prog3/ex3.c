#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    size_t size = 4096;

    // Отображаем анонимную общую память
    char *shared_mem = mmap(NULL, size,
                            PROT_READ | PROT_WRITE,
                            MAP_SHARED | MAP_ANONYMOUS,
                            -1, 0);
    if (shared_mem == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    pid_t pid = fork();
    if (pid == 0) { // дочерний процесс
        strcpy(shared_mem, "Message from child process: Hello, parent!");
        munmap(shared_mem, size);
        return 0;
    } else { // родительский процесс
        wait(NULL); // ждем завершения ребенка
        printf("Parent reads: %s\n", shared_mem);
        munmap(shared_mem, size);
    }

    return 0;
}

