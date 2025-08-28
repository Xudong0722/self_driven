# 进程控制-进程的一生

## 进程数

查看当前linux系统的pid最大值：
```
cat /proc/sys/kernel/pid_max
kernel.pid_max = 4194304
sudo sysctl kernel.pid_max
4194304
```

Linux系统中，所有的进程除了init进程都有父进程，可以通过getppid获取，最终会得到1，也就是init进程。

所以进程结构是树形结构，可以通过pstree来查看进程关心。


系统如何分配pid？

延迟重用策略，不会立马复用刚刚的进程id。
系统维护了一个bitmap，并且有一个last_pid变量，分配id时会从last_pid+1的位置开始寻找


## 进程组，会话

进程组是一组相关进程的集合，会话一组相关进程组的集合。

pid: 进程id

pgid： 进程组id，默认情况下新创建的进程会继承父进程的进程组id

sid：会话id，默认情况下新创建的进程会继承父进程的会话id


为什么要有这个东西？

进程组是为了更好的做一组任务，比如在终端中执行ls | grep test | less，他们就是一个进程组在执行一个任务，通过管道连接交互。如果我们ctrl+c停止任务，系统就会向这个进程组发送信号，组内的进程都会收到。

那么会话呢？ Linux中终端是一个稀缺资源，同一时间用户只能在一个终端中输入，如果一个终端中我需要执行某个后台作业，比如开辟一个守护进程做任务。如果还在当前这个会话中，就会受到用户输入影响，那么他可以setsid重新开启一个会话，这样讲就能从当前会话脱离出去。

当我们编译大型程序时，可以将其放在后台，在命令后添加&，这个进程组就变成了后台进程组。一个终端中可以存在多个后台进程组，但是只有一个前台进程组，负责接收用户输入，对应的信号也只会发送给前台进程组。那么一个终端中这么多进程组如何管理呢，就引入了会话这个概念，

## 进程创建

fork()调用，这个函数比较特殊，会返回两次，子进程会返回0，子进程的id会返回给父进程。所以可以根据这个返回值确定代码运行的所在进程。

```c
ret = fork();

if(ret == 0) {
    // 子进程
}else if(ret > 0) {
    //父进程
}else{
    //error handle
}
```

不能假设父子进程的调度顺序，linxu给我们提供了选项，是否先调用子进程，默认是0。这个值可以更改，我们不能假设执行顺序。

```
cat /proc/sys/kernel/sched_child_runs_first
0
```


## fork之后父子进程的内存关系

fork成功创建子进程后，子进程完全拷贝了父进程的地址空间，包括堆栈代码段等。看下面一个例子


```c
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
        free(p_int);
        sleep(6);
        printf("all var change to 100\n");
        exit(0);
    }else if(pid < 0) {
        printf("fork failed\n");
        return 1;
    }

    printf("[Parent] wait child exit.\n");
    waitpid(pid, NULL, 0);
    printf("[Parent] child have exit.\n");
    printf("[Parent] g_int: %d, local_int: %d, p_int: %d\n", g_int, local_int, *p_int);
    printf("[Parent] g_int addr: %d, local_int addr: %d\n", &g_int, &local_int);
    free(p_int);
    return 0;
}
```

可以看到，我们创建了三个变量，分别在数据段，栈，堆上，子进程中修改成100，退出后，父进程打印所有变量的值:

```
wait child exit.
Enter sub process
all var change to 100
child have exit.
g_int: 1, local_int: 1, p_int: 1
```

没有变化，所以可以看出父子进程之间互不影响。Linux系统在fork创建子进程时采用COW，写时复制，如果子进程只读之前的值，那么没有必要将新开辟一个空间复制所有的上下文。我们可以用一个程序验证一下。

```c
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

```

运行结果
```
[sudo] password for elvis: 
[Parent] g_var virt=0x64b7d1e72010 phys=39bd9c val=1

[Child] before write:
g_var virt=0x64b7d1e72010 phys=39bd9c val=1

[Child] after write:
g_var virt=0x64b7d1e72010 phys=108a53 val=100

[Parent] after child exit:
g_var virt=0x64b7d1e72010 phys=39bd9c val=1
```

可以看出，在没有修改之前，这个值所在的物理页和父进程的一致，但是修改之后，他的物理页就变了，说明这两个值存在的地方不一致，也就是写时复制发生了。

在内核代码中，会将父进程的页表设置写保护，子进程如果要修改，会触发page fault，发生拷贝。


## fork父子进程文件的关系

执行fork时，内核会复制父进程的所有文件描述符。同时父子进程会共享文件描述符的一些信息，这是非常危险的。

Linux给文件描述提供了一个标志-FD_CLOSEXEC, 同时，open函数也有一个类似的标记位O_CLOSEEXC。他们的含义是在调用exec时会自动关闭文件。比如fork子进程后执行exec的场景，子进程中对应的设置了CLOSEXEC的文件就会关闭掉。

一个例子：
root权限启动server，打开了一些root权限级别的文件和端口。降到普通用户，fork出了子进程，这时子进程是完全可以操作这些文件描述符的，这就带来了严重的安全隐患，所以上面的标记位(建议在open时就带上标记位)就派上用场了。