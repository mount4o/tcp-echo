#include "threads.h"
#include <pthread.h>

int create_thread(thread_t* thread, void* (*start_routine)(void*), void* arg) {
    return pthread_create(&thread->thread, NULL, start_routine, arg);
}

int detach_thread(thread_t* thread) {
    return pthread_detach(thread->thread);
}

void exit_thread(void) {
    pthread_exit(NULL);
}

int thread_mutex_init(thread_mutex_t* mutex) {
    return pthread_mutex_init(&mutex->mutex, NULL);
}

int thread_mutex_lock(thread_mutex_t* mutex) {
    return pthread_mutex_lock(&mutex->mutex);
}

int thread_mutex_unlock(thread_mutex_t* mutex) {
    return pthread_mutex_unlock(&mutex->mutex);
}

int thread_mutex_destroy(thread_mutex_t* mutex) {
    return pthread_mutex_destroy(&mutex->mutex);
}
