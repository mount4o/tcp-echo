#include "threads.h"
#include <windows.h>
#include <process.h>

int create_thread(thread_t* thread, void* (*start_routine)(void*), void* arg) {
    thread->handle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)start_routine, arg, 0, NULL);
    return (thread->handle == 0) ? -1 : 0;
}

int detach_thread(thread_t* thread) {
    return CloseHandle(thread->handle) ? 0 : -1;
}

void exit_thread(void) {
    _endthreadex(0);
}

int thread_mutex_init(thread_mutex_t* mutex) {
    InitializeCriticalSection(&mutex->cs);
    return 0; // Always succeeds
}

int thread_mutex_lock(thread_mutex_t* mutex) {
    EnterCriticalSection(&mutex->cs);
    return 0; // Always succeeds
}

int thread_mutex_unlock(thread_mutex_t* mutex) {
    LeaveCriticalSection(&mutex->cs);
    return 0; // Always succeeds
}

int thread_mutex_destroy(thread_mutex_t* mutex) {
    DeleteCriticalSection(&mutex->cs);
    return 0; // Always succeeds
}
