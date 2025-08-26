#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main() {

    int fd = open("test.txt", O_CREAT, 0644, "test");
    close(fd);
    return 0;
}