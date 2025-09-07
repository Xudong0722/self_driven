#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int TID = syscall(SYS_gettid);
    pid_t pid = getpid();
    printf("%d, %d\n", TID, pid);
}



