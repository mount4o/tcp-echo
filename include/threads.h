#ifndef THREADS_H
#define THREADS_H

// Define thread_t differently based on the platform
#ifdef WIN32
#include <windows.h>

typedef struct thread_t {
    HANDLE handle;
} thread_t;

typedef struct thread_mutex_t {
    CRITICAL_SECTION cs;
} thread_mutex_t;
#else
#include <pthread.h>

typedef struct thread_t {
    pthread_t thread;
} thread_t;

typedef struct thread_mutex_t {
    pthread_mutex_t mutex;
} thread_mutex_t;
#endif

int create_thread(thread_t* thread, void* (*start_routine)(void*), void* arg);
int detach_thread(thread_t* thread);
void exit_thread(void);

int thread_mutex_init(thread_mutex_t* mutex);
int thread_mutex_lock(thread_mutex_t* mutex);
int thread_mutex_unlock(thread_mutex_t* mutex);
int thread_mutex_destroy(thread_mutex_t* mutex);

#endif // THREADS_H
