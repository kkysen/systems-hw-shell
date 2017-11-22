//
// Created by kkyse on 11/9/2017.
//

#ifndef SYSTEMS_SHELL_UTIL_H
#define SYSTEMS_SHELL_UTIL_H

#include <stdio.h>

#ifndef IUTF8
    #define IUTF8 0040000
#endif

#define SHELL "shell"

#define DEBUG_SHELL true

#define shell_error() perror(SHELL)

#define einval(msg) fprintf(stderr, msg), EXIT_SUCCESS;

// needed b/c header not on school computers for some reason
// #include <sys/types.h> // done at top of file
ssize_t getline(char **lineptr, size_t *n, FILE *stream); // NOLINT

#endif //SYSTEMS_SHELL_UTIL_H
