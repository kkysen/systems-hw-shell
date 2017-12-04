//
// Created by kkyse on 11/9/2017.
//

#ifndef SYSTEMS_BUILTINS_H
#define SYSTEMS_BUILTINS_H

#include <stdbool.h>

#include "../util/utils.h"
#include "../argvv.h"

typedef struct builtin {
    const char *const name;
    int (*const call)(const Command *const command);
} Builtin;

/**
 * Get the Builtin for the \param command or NULL if none match.
 *
 * @param command the command get the associated Builtin of, if any
 * @return the Builtin or NULL if none match
 */
const Builtin *get_builtin(const Command *command);

/**
 * Run any Builtin that matches the first argument in \param command
 * and return its return value.
 * If no Builtins match, set \param ran to false.
 * If a Builtin matched and ran, set \param ran to true.
 *
 * @param [in] command the command to run
 * @param [out] ran if any Builtin ran
 * @return the return value of the Builtin that was run,
 *      or EXIT_SUCCESS if no Builtin ran.
 */
int run_builtins(const Command *command, bool *ran);

#endif //SYSTEMS_BUILTINS_H
