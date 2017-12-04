//
// Created by kkyse on 11/11/2017.
//

#ifndef SYSTEMS_CD_H
#define SYSTEMS_CD_H

#include "../util/utils.h"
#include "../argvv.h"

char CURRENT_DIR[PATH_MAX];

/**
 * Set CURRENT_DIR to be the current directory.
 */
void set_current_dir();

/**
 * Change the directory to the first argument in \param command.
 * Will only run correctly if there is a single argument after cd.
 *
 * @param [in] command the cd command
 * @return EXIT_SUCCESS if no error, anything else if error
 */
int cd_cmd(const Command *command);

#endif //SYSTEMS_CD_H
