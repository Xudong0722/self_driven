#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main()
{

    pid_t pid;
    pid = fork();

    if(pid < 0) {
        printf("error occurred!\n");
    }else if(pid == 0) {
        exit(0);
    }else{
        sleep(200);
        wait(NULL);   // 获取僵尸进程(退出的子进程的退出信息)
    }
    return 0;
}