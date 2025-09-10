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