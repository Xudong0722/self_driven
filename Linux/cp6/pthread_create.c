#include <pthread.h>
#include <stdio.h>

void* thread_worker(void *) {
    printf("I am thread worker\n");
    printf("[Sub] current pthread id: %p", (void*)pthread_self());
    pthread_exit(NULL);
}

int main() {
    pthread_t tid;
    int ret = 0;
    ret = pthread_create(&tid, NULL, &thread_worker, NULL);
    printf("[Main] current pthread id: %p", (void*)pthread_self());
    if(ret != 0) {
        printf("create thread failed\n");
        _exit(0);
    }
    pthread_join(tid, NULL);
    return 0;
}