#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>

ssize_t write(int fd, const void *buf, size_t count) {
	syscall(SYS_write, STDOUT_FILENO, "enter hook func\n", strlen("enter hook func\n"));
	return strlen("enter hook func\n");
}
