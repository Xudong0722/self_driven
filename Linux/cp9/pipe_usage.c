#include <unistd.h>
#include <stdio.h>

int main()
{
    int pipefd[2];
    pipe(pipefd);

    //pipefd[0] - read end
    //pipefd[1] - write end

    pid_t pid = fork();
    switch(pid) {
        case -1:
            return 1; // Fork failed
        case 0:{ // Child process{
            close(pipefd[1]);
            char buf[20];
            int n = read(pipefd[0], buf, sizeof(buf));
            buf[n] = '\0';
            printf("Child received: %s, cnt = %d\n", buf, n);
            close(pipefd[0]);
            return 0;
        }
        default: // Parent process pid > 0
            close(pipefd[0]);
            const char* msg = "Hello, pipe!";
            write(pipefd[1], msg, 12);
            waitpid(pid, NULL, 0); // Wait for child to finish
            close(pipefd[1]);
    }
    close(pipefd[0]);
    close(pipefd[1]);

    return 0;
}