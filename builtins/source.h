//
// Created by kkyse on 11/26/2017.
//

#ifndef SHELL_SOURCE_H
#define SHELL_SOURCE_H

#include "../argvv.h"

/**
 * Run the contents of \param filename as source.
 *
 * @param filename the name of the file to execute as source
 */
int source_file(const char *filename);

/**
 * Run the contents of the file in the first argument as source to the shell.
 *
 * @param command the source command to run
 * @return EXIT_SUCCESS if no error
 */
int source_cmd(const Command *command);

#endif //SHELL_SOURCE_H
