//
// Created by kkyse on 11/9/2017.
//

#include "util.h"

#include <stdlib.h>
#include <string.h>

char *copy_str(const char *const s) {
    const size_t size = (strlen(s) + 1) * sizeof(*s);
    char *const copy = (char *) malloc(size);
    memcpy(copy, s, size);
    return copy;
}