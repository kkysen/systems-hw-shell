//
// Created by kkyse on 11/26/2017.
//

#include "argvv.h"

#ifndef SHELL_PREPROCESSING_H
#define SHELL_PREPROCESSING_H

/**
 * Preprocess a command in its raw string form before being parsed.
 *
 * @param [in] command the string command to preprocess
 * @param [in] length the length of the \param command
 * @return the preprocessed command
 */
const char *preprocess_str_command(const char *command, size_t length);

/**
 * Preprocess a Command, returning the string form of the processed command.
 *
 * @param [in] command the Command to preprocess
 * @return the string form of the command that was preprocessed
 */
const char *preprocess_Command(const Command *command);

/**
 * Preprocess a Command by calling #preprocess_arg() on each argument
 * and replacing it with the return value of #preprocess_arg().
 *
 * @param [in, out] command the Command to preprocess
 */
void preprocess_command(const Command *command);

#endif //SHELL_PREPROCESSING_H
