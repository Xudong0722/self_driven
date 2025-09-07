#include <stdio.h>

int i;

void func()
{
    int buffer[256]; //1KB
    printf("i = %d\n", i);
    i++;
    func();
}

int main()
{
    func();
    sleep(100);
    return 0;
}