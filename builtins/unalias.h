//
// Created by kkyse on 11/26/2017.
//

#ifndef SHELL_UNALIAS_H
#define SHELL_UNALIAS_H

#include "../argvv.h"

/**
 * Unalias an alias if it exists.
 *
 * @param [in] command the unalias command
 * @return EXIT_SUCCESS if no error
 */
int unalias_cmd(const Command *command);

#endif //SHELL_UNALIAS_H
