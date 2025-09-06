#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main()
{
    char *args[] = {"/bin/l", "-l", NULL};
    if(execve("/bin/l", args, NULL) == -1) {
        printf("%d %d %s\n", errno, ENOENT, strerror(errno));
        perror("execve");
        exit(EXIT_FAILURE);
    }

    puts("Never get here.");
    exit(EXIT_SUCCESS);
}