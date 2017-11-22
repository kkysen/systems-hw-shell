//
// Created by kkyse on 11/9/2017.
//

#ifndef SYSTEMS_BUILTINS_H
#define SYSTEMS_BUILTINS_H

#ifndef _BSD_SOURCE
    #define _BSD_SOURCE 201711L
    // for strsep
#endif

#include "util.h"

typedef struct builtin {
    const char *const name;
    int (*const call)();
} Builtin;

extern const Builtin builtins[];

extern const uint num_builtins;

#endif //SYSTEMS_BUILTINS_H
