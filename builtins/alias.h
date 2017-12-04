//
// Created by kkyse on 11/26/2017.
//

#ifndef SHELL_ALIAS_H
#define SHELL_ALIAS_H

#ifndef _BSD_SOURCE
    #define _BSD_SOURCE 201711L
    // for strsep
#endif

#include "../argvv.h"

/**
 * Add an alias.
 *
 * @param [in] command the alias command in the form alias name=value,
 *      where value may be placed in quotes
 * @return EXIT_SUCCESS if no error
 */
int alias_cmd(const Command *command);

#endif //SHELL_ALIAS_H
