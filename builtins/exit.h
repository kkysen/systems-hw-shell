//
// Created by kkyse on 11/26/2017.
//

#ifndef SHELL_EXIT_H
#define SHELL_EXIT_H

#include "../argvv.h"

/**
 * Terminate the shell.
 *
 * @param [in] command ignored
 * @return EXIT_FAILURE to terminate the shell
 */
int exit_cmd(const Command *command);

#endif //SHELL_EXIT_H
