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