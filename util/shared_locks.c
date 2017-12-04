//
// Created by kkyse on 11/30/2017.
//

#include "shared_locks.h"

#include "shared_memory.h"

Mutex *new_shared_Mutex() {
    Mutex *const mutex = (Mutex *) malloc_shared(sizeof(Mutex));
    MutexAttr mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(mutex, &mutex_attr);
    return mutex;
}

void free_shared_Mutex(Mutex *const mutex) {
    pthread_mutex_destroy(mutex);
    free_shared(mutex, sizeof(Mutex));
}

#if (SPIN_LOCK)

SpinLock *new_shared_SpinLock() {
    SpinLock *const lock = (SpinLock *) malloc_shared(sizeof(SpinLock));
    pthread_spin_init(lock, PTHREAD_PROCESS_SHARED);
    return lock;
}

void free_shared_SpinLock(SpinLock *const lock) {
    pthread_spin_destroy(lock);
    free_shared((void *) lock, sizeof(SpinLock));
}

#endif