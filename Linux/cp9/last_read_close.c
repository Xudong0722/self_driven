#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void sighandler(int sig);
int main()
{
    int pipefd[2];
    pipe(pipefd);

    //pipefd[0] - read end
    //pipefd[1] - write end

    if(signal(SIGPIPE, sighandler) == SIG_ERR) {
        fprintf(stderr, "signal error (%s)\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    switch(pid) {
        case -1:
            return 1; // Fork failed
        case 0:{ // Child process{
            fprintf(stdout, "[CHILD] I will close the last read end of pipe\n");
            close(pipefd[0]);
            exit(EXIT_FAILURE);
        }
    }
    close(pipefd[0]);
    sleep(1); // Ensure child runs first

    int ret = 0;
    ret = write(pipefd[1], "Hello, pipe!", 12);
    if(ret == -1) {
        fprintf(stderr, "[PARENT] write error (%s)\n", strerror(errno));
    } else {
        fprintf(stdout, "[PARENT] write %d bytes to pipe\n", ret);
    }
    return 0;
}

void sighandler(int sig) {
    if(sig == SIGPIPE) {
        fprintf(stderr, "[PARENT] received SIGPIPE signal\n");
    }
}