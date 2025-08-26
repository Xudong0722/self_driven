#include <stdio.h>

int global_uninit;          // 放在 .bss
int global_init = 42;       // 放在 .data
static int big_array[1000000]; // 放在 .bss，大约 4MB

int main() {
    printf("global_uninit=%d, global_init=%d\n", global_uninit, global_init);
    return 0;
}

