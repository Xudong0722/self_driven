#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <stdio.h>

int g_int = 1;
int main()
{
    int* p_int = malloc(sizeof(int));
    *p_int = 1;

    int local_int = 1;
    pid_t pid = fork();
    if(pid == 0) {
        printf("[Child] Enter sub process\n");
        local_int = 100;
        g_int = 100;
        *p_int = 100;
        printf("[Child] g_int: %d, local_int: %d, p_int: %d\n", g_int, local_int, *p_int);
        printf("[Child] g_int addr: %p, local_int addr: %p\n", &g_int, &local_int);
        free(p_int);
        sleep(6);
        //printf("all var change to 100\n");
        exit(0);
    }else if(pid < 0) {
        printf("fork failed\n");
        return 1;
    }

    printf("[Parent] wait child exit.\n");
    waitpid(pid, NULL, 0);
    printf("[Parent] child have exit.\n");
    printf("[Parent] g_int: %d, local_int: %d, p_int: %d\n", g_int, local_int, *p_int);
    printf("[Parent] g_int addr: %p, local_int addr: %p\n", &g_int, &local_int);
    free(p_int);
    return 0;
}