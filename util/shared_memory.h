//
// Created by kkyse on 11/29/2017.
//

#ifndef SHELL_SHARED_MEMORY_H
#define SHELL_SHARED_MEMORY_H

#include <stdlib.h>

/**
 * Allocate anonymous shared mmap'ed memory of size \param size.
 *
 * @param size the size of memory to map
 * @return the pointer to the memory
 */
void *malloc_shared(size_t size);

/**
 * Free anonymous shared mmap'ed memory of size \param size.
 *
 * @param addr the address to free
 * @param size the size that was allocated for the \param addr
 * @return 0 if success, -1 if error
 */
int free_shared(void *addr, size_t size);

#endif //SHELL_SHARED_MEMORY_H
