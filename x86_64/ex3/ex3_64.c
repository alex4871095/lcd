#include <stdio.h>

void __attribute__((cdecl)) dummy_function(int a, int b, int c, int d, int e, int f)
{
	;
}

int main() {
    int x = 1, y = 2, z = 3, u = 4, v = 5, w = 6;
    dummy_function(x, y, z, u, v, w);
    return 0;
}

