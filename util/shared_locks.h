//
// Created by kkyse on 11/30/2017.
//

#ifndef SHELL_MUTEXES_H
#define SHELL_MUTEXES_H

#define SPIN_LOCK false

#include <pthread.h>

typedef pthread_mutex_t Mutex;

typedef pthread_mutexattr_t MutexAttr;

/**
 * Create a new Mutex in mmap anonymous shared memory.
 *
 * @return the newly-mmap'ed Mutex
 */
Mutex *new_shared_Mutex();

/**
 * Free a Mutex \param mutex in mmap anonymous shared memory.
 *
 * @param mutex the shared Mutex to free
 */
void free_shared_Mutex(Mutex *mutex);

#if (SPIN_LOCK)

typedef pthread_spinlock_t SpinLock;

/**
 * Create a new SpinLock in mmap anonymous shared memory.
 *
 * @return the newly-mmap'ed SpinLock
 */
SpinLock *new_shared_SpinLock();

/**
 * Free a SpinLock \param lock in mmap anonymous shared memory.
 *
 * @param lock the shared SpinLock to free
 */
void free_shared_SpinLock(SpinLock *lock);

#endif

#endif //SHELL_MUTEXES_H
