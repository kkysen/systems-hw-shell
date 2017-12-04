//
// Created by kkyse on 12/3/2017.
//

#ifndef SHELL_PID_H
#define SHELL_PID_H

#include "../argvv.h"

/**
 * Print the shell's process ID.
 *
 * @param [in] command ignored
 * @return EXIT_SUCCESS if success, anything else if error
 */
int pid_cmd(const Command *command);

#endif //SHELL_PID_H
