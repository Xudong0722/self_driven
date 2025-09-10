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

### 线程的分离- pthread_detach

如果我们不关心线程的返回值，线程支持分离操作，这样在线程退出后，系统将负责回收线程的资源。

- 分离线程可以由其他线程操作，线程自身也可以分离自己
- 线程分离后，exit对其还是生效的
- 不仅可以用pthread_detach来分离线程，也可以在线程创建的时候将线程的属性设定为已分离。

```c
#include <pthread.h>
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
int pthread_attr_getdetachstate(pthread_attr_t *attr, int *detachstate);
```


## 互斥量

为什么需要互斥量？

多线程访问同时读写临界区数据的话，如果不是原子操作，会造成数据错误。

```c
#include <pthread.h>

//静态初始化
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//动态初始化
int pthread_mutex_init(pthread_mutex_t *restrict mutex, ...);
```

互斥量销毁：
- 使用PTHREAD_MUTEX_INITIALIZER初始化的互斥量不需要销毁
- 被锁定的互斥量不能销毁，返回EBUSY错误码
- 和条件变量配合使用的不能销毁，返回EBUSY错误码

如何设置互斥锁的属性，pthread_mutexattr_settype
```c
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define _GNU_SOURCE
pthread_mutex_t mutex;

int main() {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    //pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_TIMED_NP);  //normal mutex
    //pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK_NP);  //return error when you lock locked mutex
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);  //with internal counter 
    pthread_mutex_init(&mutex, &attr);

    int a = 100;
    int res = pthread_mutex_lock(&mutex);
    a = 101;
    printf("res = %d\n", res);
    res = pthread_mutex_lock(&mutex);//deadlock when PTHREAD_MUTEX_TIMED_NP
    printf("res = %d\n", res);
    printf("a = %d, strerror = %s\n", a, strerror(res));  
    pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&mutex);
    return 0;
}
```

## 互斥量的性能

传统的同步手段：
- 进入临界区之前，申请锁，执行系统调用，查看是否存在竞争。
- 离开临界区释放锁时，再次执行系统调用，查看是否存在需要唤醒的线程。

这两次系统调用在竞争不激烈的情况下可能会有点浪费：进入临界区不存在竞争，离开临界区没有待唤醒的线程。

linux上互斥量采用了futex(fast user space mutex)，主要流程如下：

```c
c==0: 没有上锁
c==1: 上锁了，没有线程在等待
c==2: 上锁了，有线程在等待
void lock(mutex& lock) 
{
    int c;
    c = cmpxchg(lock, 0, 1); //如果lock的值等于0，将lock改为1，并返回0，否则直接返回lock的值
    if(c == 0) return  //如果lock等于0，直接返回，否则lock已经加锁，继续往下
    do
    {
        //1.如果有线程在等待这个锁了，调用futex_wait
        //2.如果只有一个线程占用，lock的值等于1，我们将其改为2，因为我们在等待
        if(c == 2 || cmpxchg(lock, 1, 2) != 0) 
        {
            //执行futex_wait的时候如果lock不为2就会立即返回
            futex_wait(lock, 2);
        }
    //如果有线程unlock了，0，1,2都有可能
    //如果是0，就可以退出循环返回了， lock改为2
    //如果是1，继续循环，循环内部会改成2
    //如果是2，循环内部也是2
    }while((c = cmpxchg(lock, 0, 2)) != 0);
}

void unlock(mutex* lock) 
{
    //当前lock值为1或者2
    //atomic_dec(x) 将x的值减去1，并返回原始值
    if(atomic_dec(lock) != 1) {
        //lock为2才会进来，所以没有竞争的话会直接返回
        lock = 0;  //修改为0，唤醒其他线程
        futex_wake(lock, 1);  //唤醒一个等待者
    }
}
```

没有竞争的mutex是非常快的， 在我的电脑3.5GHz,i5-13600KF上一次加锁解锁得到的结果是2ns左右。

```c
int futex_wait(int* uaddr, int val);
int futex_wake(int *uaddr, int n);
```

在内核里，futex 的等待队列是通过一个 哈希表 (futex hash table) 实现的：

内核不会为每个用户态地址单独建一个等待队列，而是把 用户态地址 (uaddr) + 进程地址空间 (mm) 作为 key。

这个 key 会被哈希到内核中的一个 bucket（哈希桶），每个 bucket 里是一个链表，存放多个 futex 的等待队列节点。

换句话说：
- 同一个进程，同一个 &lock 地址 → 哈希到相同的 bucket → 等待线程都在同一个等待队列。
- 不同进程，如果共享内存里的同一个地址（通过 mmap/shm） → 也会在同一个等待队列。
- 不同变量（不同 uaddr） → 哈希到不同位置（可能同 bucket，但链表区分）。


根据上面的分析，如果临界区申请不到互斥量就会让出CPU，陷入阻塞，等待唤醒。
如果临界区非常小，竞争不激烈，忙-等待的策略就会优于上面互斥锁的策略，也就是使用自旋锁。


## 互斥量的公平性

首先，给出结论，互斥量不是公平的，没有做到先来先服务。原因有下：

线程从从阻塞到被唤醒并不是自动获得互斥量，还是要通过原子操作修改lock，这就还是会出现竞争的情况，如果竞争失败，还是会调用futex_wait进入队尾，加剧不公平的情况。

所以，互斥量只能说是大体上的公平，绝对的公平在这里是没有必要的，且会影响性能。因为线程是由系统调度的，在这一层面上，谁能保证哪个线程会先调用加锁操作呢？

## 自适应锁

前面互斥锁一节中提到我们可以给mutex设置属性，当时只提到了三种：
- PTHREAD_MUTEX_TIMED_NP
- PTHREAD_MUTEX_ERRORCHECK_NP
- PTHREAD_MUTEX_RECURSIVE_NP

还有一种自适应锁：
- PTHREAD_MUTEX_ADAPTIVE_NP

也被称为fast mutex，前面提到，互斥锁的机制是加锁失败后陷入阻塞让出CPU，而自旋锁是继续尝试加锁直到获取锁，这在临界区很小的情况下可能比互斥量更优，但也要注意，自旋锁使用不当情况下副作用很大，要注意：

- 确保临界区尽可能小，不要有系统调用
- 不要调用sleep
- 内存操作需要考虑操作内存的大小，是否会引起缺页中断

有没有可能将两者的优势结合一下，自适应锁出现了：

如果竞争锁失败，就会和自旋锁一样继续尝试获取，但如果过了一定时间还没有申请到锁，就会放弃尝试，让出CPU并等待。这里的等待时间会根据实际情况的反馈智能地调整，当然，这个时间也有上限，自适应次数是有上限的。


## 死锁与活锁

1. 存在多个互斥量的情况下，如何避免死锁？

- 线程申请加锁的顺序一致就可以避免
- 尝试获取锁，没有获取就返回， trylock, timedlock

2. 什么是活锁？

生动的例子：两个迎面走来，互让身位，结果每次都是相反的方向移动，堵了好一会才通行。

对应到代码里：
线程1获取锁a，线程2获取锁b，
- 然后线程1获取锁b失败释放锁a，线程2获取锁a失败释放锁b
- 线程1获取锁a，线程2获取锁b
- ...

某一时刻，线程1同时获取锁a和锁b，结束。。。这种情况就是活锁，比较耗费性能。不过我觉得这主要是代码的问题。。。