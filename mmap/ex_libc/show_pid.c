#include <stdio.h>
#include <unistd.h>

int main() {
    printf("PID=%d\n", getpid());
    printf("Press Enter to continue...\n");
    getchar();
    return 0;
}

