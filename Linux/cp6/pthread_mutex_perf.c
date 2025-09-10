#include <pthread.h>
#include <time.h>
#include <stdio.h>

const int N = 10000000;

int main()
{   
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(int i = 0; i<N; i++){
        pthread_mutex_lock(&mutex);
        pthread_mutex_unlock(&mutex);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    long long elapsed_ns = (end.tv_nsec - start.tv_nsec) +
      (end.tv_sec - start.tv_sec) * 1000000000LL;
    
    int per_op_ns = elapsed_ns / N;

    printf("total time(ns) is %lld\nper lock and unlock without race is %d ns.\n", elapsed_ns, per_op_ns);
    return 0;
}