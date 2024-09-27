#ifndef THREADS_H
#define THREADS_H

// Define thread_t and thread_mutex_t differently based on the platform
#ifdef WIN32
#include <windows.h>

/**
 * @struct thread_t
 * @brief Thread handle for Windows platforms.
 *
 * This structure represents a thread handle on Windows platforms. 
 * It encapsulates the Windows-specific `HANDLE` type.
 */
typedef struct thread_t {
    HANDLE handle;
} thread_t;

/**
 * @struct thread_mutex_t
 * @brief Mutex structure for Windows platforms.
 *
 * This structure represents a mutex on Windows platforms. 
 * It encapsulates the Windows-specific `CRITICAL_SECTION` type.
 */
typedef struct thread_mutex_t {
    CRITICAL_SECTION cs;
} thread_mutex_t;

#else
#include <pthread.h>

/**
 * @struct thread_t
 * @brief Thread handle for POSIX-compliant platforms.
 *
 * This structure represents a thread on POSIX-compliant platforms. 
 * It encapsulates the POSIX-specific `pthread_t` type.
 */
typedef struct thread_t {
    pthread_t thread;
} thread_t;

/**
 * @struct thread_mutex_t
 * @brief Mutex structure for POSIX-compliant platforms.
 *
 * This structure represents a mutex on POSIX-compliant platforms. 
 * It encapsulates the POSIX-specific `pthread_mutex_t` type.
 */
typedef struct thread_mutex_t {
    pthread_mutex_t mutex;
} thread_mutex_t;
#endif

/**
 * @brief Creates a new thread.
 *
 * This function creates a new thread that starts executing the specified 
 * `start_routine` with the given argument `arg`.
 * 
 * @param thread Pointer to the thread_t structure where the thread handle will be stored.
 * @param start_routine Function pointer to the routine that the thread will execute.
 * @param arg Pointer to the argument that will be passed to the `start_routine`.
 * @return 0 on success, or a non-zero error code on failure.
 */
int create_thread(thread_t* thread, void* (*start_routine)(void*), void* arg);

/**
 * @brief Detaches a thread.
 *
 * This function detaches a thread, allowing it to run independently and 
 * automatically free its resources upon completion.
 * 
 * @param thread Pointer to the thread_t structure representing the thread to be detached.
 * @return 0 on success, or a non-zero error code on failure.
 */
int detach_thread(thread_t* thread);

/**
 * @brief Exits the calling thread.
 *
 * This function terminates the calling thread. It does not return and should 
 * be called at the end of a thread's execution if the thread was not detached.
 */
void exit_thread(void);

/**
 * @brief Initializes a mutex.
 *
 * This function initializes a mutex, making it ready for use in locking and 
 * unlocking critical sections of code.
 * 
 * @param mutex Pointer to the thread_mutex_t structure to be initialized.
 * @return 0 on success, or a non-zero error code on failure.
 */
int thread_mutex_init(thread_mutex_t* mutex);

/**
 * @brief Locks a mutex.
 *
 * This function locks the specified mutex, blocking the calling thread if 
 * the mutex is already locked by another thread. The mutex should be 
 * unlocked by the same thread that locked it.
 * 
 * @param mutex Pointer to the thread_mutex_t structure representing the mutex to be locked.
 * @return 0 on success, or a non-zero error code on failure.
 */
int thread_mutex_lock(thread_mutex_t* mutex);

/**
 * @brief Unlocks a mutex.
 *
 * This function unlocks the specified mutex, allowing other threads to 
 * acquire the mutex if they are waiting for it.
 * 
 * @param mutex Pointer to the thread_mutex_t structure representing the mutex to be unlocked.
 * @return 0 on success, or a non-zero error code on failure.
 */
int thread_mutex_unlock(thread_mutex_t* mutex);

/**
 * @brief Destroys a mutex.
 *
 * This function destroys the specified mutex, freeing any resources that 
 * were allocated for it. The mutex should not be used after it has been destroyed.
 * 
 * @param mutex Pointer to the thread_mutex_t structure representing the mutex to be destroyed.
 * @return 0 on success, or a non-zero error code on failure.
 */
int thread_mutex_destroy(thread_mutex_t* mutex);

#endif // THREADS_H
