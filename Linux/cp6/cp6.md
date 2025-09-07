# 理解Linux线程

## 进程id和线程id

Linux上调度的最小单元就是线程-task_struct，他也有自己的进程描述符。因为线程和进程的一个很大的区别就是在创建的时候，进程是会拷贝父进程的空间的，而多线程是共享一个进程内的数据，线程需要这个描述符是因为他需要能被调度，所以描述符中会存放他自己的上下文，堆栈信息等。

POSIX标准要求一个进程内的多线程调用getpid能返回同样的pid，并且每个线程的都有自己的线程id。

如何获取线程id？
```c
#include <sys/syscall.h>
int TID = syscall(SYS_gettid);
```

每个线程的进程描述符(task_struct)中都有一个group_leader指针，指向主线程，主线的线程id等于当前进程的id。

## pthread库

### 线程创建- pthread_create

```c
#include <pthread.h>

       int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
```
```c
#include <pthread.h>
#include <stdio.h>

void* thread_worker(void *) {
    printf("I am thread worker\n");
    pthread_exit(NULL);
}

int main() {
    pthread_t tid;
    int ret = 0;
    ret = pthread_create(&tid, NULL, &thread_worker, NULL);
    if(ret != 0) {
        printf("create thread failed\n");
        _exit(0);
    }
    pthread_join(tid, NULL);
    return 0;
}
```

我们来看看这个tid是什么，它是线程id么？实际上，他是一个地址，指向地址空间中为当前线程分配的地址。

这里简单提一下线程栈的大小,大概是8MB， 也可以通过*ulimit -s*来查看，下面这个程序也能大概看出来。

```c
#include <stdio.h>

int i;

void func()
{
    int buffer[256]; //1KB
    printf("i = %d\n", i);
    i++;
    func();
}

int main()
{
    func();
    sleep(100);
    return 0;
}
```

所以，如果要使用这个id，只能在一个进程内做区分，因为不同进程中，不同线程通过pthread_self获取到的pthread_t可能是一样的，它简单地通过地址来区分不同的线程。所以我们最号还是使用前面一节提到的syscall来获取task_struct中的pid。这有很多好处：

1. 任意时刻全局唯一
2. /proc/pid/task/tid 方便地查看线程消息
3. 系统命令ps也是查看的pid，很好地对应起来，方便我们调试


### 线程退出- pthread_exit

三种情况，线程会终止：
1. 创建线程的start_routine函数执行了return
2. 当前线程调用了pthread_exit
3. 其他线程通过pthread_cancel取消了该线程
4. 线程组中的任何一个线程调用了exit(会调用exit_group)
5. 主线程调用return，和调用exit一个效果(会调用exit_group)

函数签名：
```c
#include <pthread.h>

void pthread_exit(void *retval);
```

其中，retval是当前线程的临终遗言，其他线程可以通过pthread_join接收到这个地址查看遗言。此外，线程创建时的start_routine的返回值也是void*类型，它也是这个作用。

```c
#include <pthread.h>
#include <stdio.h>

int data;

void* thread_worker(void *) {
    printf("I am thread worker\n");
    printf("[Sub] current pthread id: %p\n", (void*)pthread_self());
    data = 101;
    pthread_exit((void*)&data);
}

int main() {
    pthread_t tid;
    int ret = 0;
    ret = pthread_create(&tid, NULL, &thread_worker, NULL);
    printf("[Main] current pthread id: %p\n", (void*)pthread_self());
    if(ret != 0) {
        printf("create thread failed\n");
        _exit(0);
    }
    void* retval = NULL;
    pthread_join(tid, &retval);
    printf("The retval of sub thread is %d\n", *((int*)retval));
    return 0;
}
```

上面这个例子就展示了pthread_exit的用法，将修改的data的地址放在retval中，主线程可以通过pthread_join获得它的遗言。

### 线程连接- pthread_join

线程的join和进程之间的wait关系不一致，进程只能父进程等待子进程退出，而线程是平级关系，线程可以等待指定的线程join，但是线程不能等待任意一个线程join(比如第三方库创建的线程理论上和我们的程序没有关系，我们没有理由join这些线程)。

pthread_join判断是否可以连接依赖struct pthread中的一个字段 struct pthread* joinid

1. NULL， 表示该线程可join
2. 指向自身，表示该线程会自己释放资源
3. 指向其他线程，表示其他线程已经join此线程

两个线程同时对一个joinable的线程执行join操作-只有一个会成功，join是原子操作-cas实现

执行pthread_join的必要性：

对已经退出的线程执行pthread_join非常关键，因为线程退出后并不会释放资源，那么它占有的的资源也不会得到释放，实际上这就是内存泄漏，如果创建的线程都不执行连接操作，久而久之，内存会耗尽。
而线程组的其他线程对退出线程执行phtread_join后，会调用__free_tcb，释放线程占有的控制块。注意，线程栈并不会立马释放，因为线程库设计的时候考虑了复用的case，如果join完后又会创建线程，频繁的munmap和mmap会影响性能，所以线程库设计成将退出线程的栈缓存起来放入了一个链表中，新的请求会在栈缓存链表中寻找合适的栈，如果有，就将其分配给新创建的线程。这个缓存也是有上限的，达到上限，就会归还一部分空间给系统。

注意，这里说的线程栈是指pthread_create时在用户空间通过mmap申请的栈空间，默认是8MB。需要通过munmap释放。


