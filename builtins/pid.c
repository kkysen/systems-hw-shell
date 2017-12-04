//
// Created by kkyse on 12/3/2017.
//

#include "pid.h"

#include <stdio.h>
#include <unistd.h>

int pid_cmd(const Command *const command) {
    printf("process ID: %d\n", getpid());
    return EXIT_SUCCESS;
}