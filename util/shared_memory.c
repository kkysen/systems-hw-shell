//
// Created by kkyse on 11/29/2017.
//

#include "shared_memory.h"

#include <sys/mman.h>

void *malloc_shared(const size_t size) {
    void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED) {
        return NULL;
    }
    return addr;
}

int free_shared(void *const addr, const size_t size) {
    return munmap(addr, size);
}