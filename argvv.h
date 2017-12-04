//
// Created by kkyse on 11/25/2017.
//

#ifndef SHELL_ARGVV_H
#define SHELL_ARGVV_H

#include <sys/types.h>
#include <stdlib.h>

#define is_delim(c) ((c) == ';' || (c) == '|' || (c) == '>' || (c) == '<')

typedef struct str_command {
    const char *cmd;
    const char *delimiter;
} _StrCommand;

typedef struct command {
    const char **argv;
    size_t argc;
    size_t _max_argc;
    const char *pre_delimiter;
    const char *post_delimiter;
    char *_buf;
    size_t _buf_len;
} Command;

typedef struct commands {
    Command *argvv;
    size_t argvc;
    size_t _max_argvc;
    _StrCommand *_str_commands;
} Commands;

extern Commands all_commands;

/**
 * Parses a \param line into the argvv from Commands#argvv_ptr.
 * Commands#argvc_ptr and Commands#max_argvc_ptr are also updated accordingly.
 *
 * If there is no error, returns 0.
 * If there was a syntax error in the \param line, the ~index (NOT) of the syntax error is returned.
 *
 * Commands#argvv_ptr is not null-terminated.
 *
 * @param [in] line the line to parse
 * @param [out] commands the struct containing pointers to the list of Commands that will be set
 * @return the ~index (NOT) of any syntax error in \param command, or 0 if none
 */
ssize_t parse_commands(char *line, Commands *commands);

/**
 * Free and zero/NULL the fields of the \param commands.
 *
 * @param [out] commands ptr to the Commands to free
 */
void free_Commands(Commands *commands);

#define using_argvv(commands, const_Command, const_ptr) \
    const_Command Command *const_ptr argvv = (commands)->argvv; \
    const size_t argvc = (commands)->argvc

#define using_argvv_cc() using_argvv(commands, const, const)

/**
 * Return a newly-allocated string representation of a \param command.
 *
 * @param command the command to convert to a string
 * @return a newly-allocated string repr. of \param command
 */
char *Command_to_string(const Command *command);

/**
 * Return a newly-allocated string representation of \param commands.
 *
 * @param commands the commands to convert to a string
 * @return a newly-allocated string repr. of \param commands
 */
char *Commands_to_string(const Commands *commands);

/**
 * Print a single \param command.
 *
 * @param [in] command the command to print
 */
void print_Command(const Command *command);

/**
 * Print all the commands in \param commands.
 *
 * @param [in] commands the commands to print
 */
void print_Commands(const Commands *commands);

#endif //SHELL_ARGVV_H