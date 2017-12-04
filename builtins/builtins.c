//
// Created by kkyse on 11/9/2017.
//

#include <string.h>
#include <stdio.h>
#include "builtins.h"

#include "cd.h"
#include "exit.h"
#include "alias.h"
#include "unalias.h"
#include "source.h"
#include "pid.h"

static const Builtin builtins[] = {
        {"cd", &cd_cmd},
        {"exit", &exit_cmd},
        {"alias", &alias_cmd},
        {"unalias", &unalias_cmd},
        {"source", &source_cmd},
        {"pid", &pid_cmd},
};

const Builtin *get_builtin(const Command *const command) {
    const char *const name = command->argv[0];
//    #pragma unroll
    for (size_t i = 0; i < arraylen(builtins); ++i) {
        const Builtin builtin = builtins[i];
        if (strcmp(name, builtin.name) == 0) {
            return builtins + i;
        }
    }
    return NULL;
}

int run_builtins(const Command *const command, bool *const ran) {
    const char *const name = command->argv[0];
//    printf("looking for builtin: %s\n", name);
    // check if program name matches any builtins
    // should use a HashMap, but too complicated
//    #pragma unroll
    for (size_t i = 0; i < arraylen(builtins); ++i) {
        const Builtin builtin = builtins[i];
        if (strcmp(name, builtin.name) == 0) {
            *ran = true;
//            printf("found builtin\n");
            return builtin.call(command);
        }
    }
    
    *ran = false;
    return EXIT_SUCCESS;
}