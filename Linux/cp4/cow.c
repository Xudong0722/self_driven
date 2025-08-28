#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/wait.h>

// 读取虚拟地址 va 对应的物理页号（PFN）
unsigned long virt2phys(void* va) {
    int page_size = getpagesize();
    unsigned long vpn = (unsigned long)va / page_size;  // 虚拟页号
    unsigned long offset = vpn * sizeof(uint64_t);

    int fd = open("/proc/self/pagemap", O_RDONLY);
    if (fd < 0) { perror("open pagemap"); exit(1); }

    uint64_t entry;
    if (pread(fd, &entry, sizeof(entry), offset) != sizeof(entry)) {
        perror("pread");
        exit(1);
    }
    close(fd);

    if (!(entry & (1ULL << 63))) return 0; // present bit
    return entry & ((1ULL << 55) - 1);     // PFN
}

static int g_var = 1;  // 全局变量

int main() {
    printf("[Parent] g_var virt=%p phys=%lx val=%d\n", 
           &g_var, virt2phys(&g_var), g_var);

    pid_t pid = fork();
    if (pid == 0) {
        printf("\n[Child] before write:\n");
        printf("g_var virt=%p phys=%lx val=%d\n", 
               &g_var, virt2phys(&g_var), g_var);

        // 修改，触发 COW
        g_var = 100;
        
        printf("\n[Child] after write:\n");
        printf("g_var virt=%p phys=%lx val=%d\n", 
               &g_var, virt2phys(&g_var), g_var);
        sleep(5);
        exit(0);
    } else {
        waitpid(pid, NULL, 0);
        printf("\n[Parent] after child exit:\n");
        printf("g_var virt=%p phys=%lx val=%d\n", 
               &g_var, virt2phys(&g_var), g_var);
    }
    return 0;
}
