//
// Created by kkyse on 11/27/2017.
//

#ifndef SHELL_SHELL_LOOP_H
#define SHELL_SHELL_LOOP_H

#include <stdbool.h>
#include "argvv.h"

/**
 * Repeatedly run the REPL loop of the shell.
 *
 * @param [in, out] commands where the commands are stored
 * @param [in] user if the shell is running for the user
 */
void loop(Commands *commands, bool user);

#endif //SHELL_SHELL_LOOP_H
