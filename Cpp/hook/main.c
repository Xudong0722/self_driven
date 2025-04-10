#include <unistd.h>
#include <string.h>

int main() {
	write(STDOUT_FILENO, "hello world\n", strlen("hello world\n"));
	return 0;
}
