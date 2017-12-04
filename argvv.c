//
// Created by kkyse on 11/25/2017.
//

#include "argvv.h"

#include <stdio.h>
#include <string.h>

#include "util/str_utils.h"
#include "preprocessing.h"
#include "util/utils.h"

Commands all_commands = {
        .argvv = NULL,
        .argvc = 0,
        ._max_argvc = 0,
        ._str_commands = NULL,
};

#define skip_whitespace() while ((c = *line++) && is_whitespace(c)); --line

#define check_quote(quote_char, c) \
    if (is_quote(c)) { \
        if ((quote_char) == 0) { \
            (quote_char) = c; \
        } else if ((quote_char) == (c)) { \
            (quote_char) = 0; \
        } \
    }

#define i() (line - start)

/**
 * Return number of commands in a single line.
 * ;, |, <, > are command delimiters (the only ones allowed now).
 * Anything in quotes are skipped.
 *
 * If there is a syntax error in the line,
 * ~i is returned, where i is the index of the syntax error.
 *
 * @param [in] line the line
 * @return the number of commands in the line or the ~index (NOT) of the syntax error
 */
static ssize_t find_num_commands(const char *line) {
    const char *const start = line;
    char c;
    skip_whitespace();
    
    if (!c) {
        return 0;
    }
    if (is_delim(c)) {
        return ~i();
    }
    
    char quote_char = 0;
    size_t num_commands = 0;
    char last_delim = 0;
    while ((c = *line++)) {
        check_quote(quote_char, c);
        if (quote_char) {
            continue;
        }
        if (!is_delim(c)) {
            continue;
        }
        if ((last_delim == '<' || last_delim == '>') && last_delim == c) {
            // can't have multiple redirects in a row
            return ~i();
        }
        last_delim = c;
        // can't have 2 delims in a row
        skip_whitespace();
        if (!c) {
            break;
        }
        if (is_delim(c)) {
            return ~i();
        }
        num_commands++;
    }
    
    // check if ends with anything but ';'
    --line;
    while ((c = *--line) && is_whitespace(c));
    if (quote_char || (is_delim(c) && c != ';')) {
        return ~i();
    }
    
    return num_commands + 1;
}

#undef i

/**
 * Return a newly-allocated string command by either copying the old one
 * or modifying it and returning a new string.
 *
 * Unquotes the command.
 *
 * @param [in, out] command the original string command
 * @param [in] length the length of the \param command
 * @return a newly-allocated string command
 */
static const char *get_str_command(char *command, const size_t length) {
    command = unquote(strip_n(command, length));
    const char *preprocessed_command = preprocess_str_command(command, strlen(command));
    if (command == preprocessed_command) {
        preprocessed_command = str_copy_n(preprocessed_command, length);
    }
    return preprocessed_command;
}

/**
 * Parses a line into multiple commands.
 * Anything in quotes is skipped.
 *
 * This should be called after #find_num_commands(),
 * so \param s should be correct and \param commands should be the right size.
 *
 * @param [in] line the line to parse
 * @param [out] commands the array of commands
 */
static void parse_commands_to_strings(char *line, _StrCommand *commands) {
    char *start = line;
    char c;
    skip_whitespace();
    
    if (!c) {
        return;
    }
    
    char quote_char = 0;
    while ((c = *line++)) {
        check_quote(quote_char, c);
        if (quote_char) {
            continue;
        }
        if (!is_delim(c)) {
            continue;
        }
        commands->delimiter = str_copy_n(line - 1, 1);
        commands->cmd = get_str_command(start, line - 1 - start);
        commands++;
        start = line;
    }
    
    // add trailing command
    commands->cmd = get_str_command(start, line - 1 - start);
    commands->delimiter = str_copy("");
}

/**
 * Return number of args in a single command and assigns \param stripped length.
 * Handles extra whitespace.
 *
 * @param [in] command the command
 * @param [out] stripped_length the length of the command once extra whitespace has been stripped
 * @return the number of args in the command
 */
static size_t find_num_args(const char *command, size_t *stripped_length) {
    size_t argc = 0;
    const char *s = command;
    if (is_newline(s[0]) && (s[1] == 0 || (is_newline(s[1]) && s[2] == 0))) {
        // no input, only newline
        // handles '\n' and '\r'
        *stripped_length = 0;
        return 0;
    }
    char c;
    char quote_char = 0;
    size_t length = 0;
    for (; (c = *s++); ++length) {
        // skip anything in quotes
        check_quote(quote_char, c);
        if (quote_char) {
            continue;
        }
        
        if (is_whitespace(c)) {
            while ((c = *s++) && is_whitespace(c));
            if (!c) {
                break;
            }
            --s;
            ++argc;
        }
    }
    *stripped_length = length;
    return argc + 1;
}

/**
 * Copy and unquote the argument.
 *
 * @param arg the argument
 * @param length the length of the argument
 * @return the copied and unquoted argument
 */
static char *get_arg(char *const arg, const size_t length) {
    const char *const unquoted = unquote_n(arg, length);
    if (arg == unquoted) {
        return str_copy_n(unquoted, length);
    } else {
        return str_copy_n(unquoted, length - 2);
    }
}

/**
 * Parses a command into its args, skipping extra whitespace (' ', '\t', '\n', '\r').
 * Anything inside quotes isn't parsed at all.
 * Uses a temporary buffer \param buf to hold the stripped command, but no references to it are made.
 *
 * This should be called after #find_num_args() to determine how large \param argv must be.
 *
 * @param [in] command the command to parse
 * @param [out] argv the null-terminated argument vector to place the newly-allocated args into
 * @param [out] buf the buffer to temporarily hold the whitespace-stripped command
 */
static void parse_command_with_buf(const char *command, const char **argv, char *buf) {
    char *start = buf;
    char c;
    char quote_char = 0;
    while ((c = *command++)) {
        // don't parse anything inside quotes
        check_quote(quote_char, c);
        if (quote_char) {
            *buf++ = c;
            continue;
        }
        
        if (is_whitespace(c)) {
            while ((c = *command++) && is_whitespace(c));
            --command;
            if (!c) {
                // skip trailing whitespace
                break;
            }
            c = 0;
        }
        
        *buf++ = c;
        if (!c) {
            *argv++ = get_arg(start, buf - 1 - start);
            start = buf;
        }
    }
    *argv++ = get_arg(start, buf - start);
    *argv = NULL;
}

/**
 * Set the fields of a Command from parsing a new \param command.
 *
 * The Command#delimiter field is not set.  It should be set separately.
 *
 * Command#argv and Command#buf are grown if needed.
 * Their new lengths are set in Command#max_argc and Command#buf_len.
 *
 * The Command is also recursively preprocessed using #preprocess_Command()
 * until #preprocess_Command() returns the same str command,
 * meaning there are no more preprocessing changes.
 *
 * @param [in] command_to_parse the command to parse
 * @param [out] command the Command containing the argv, argc, and buf
 */
static void parse_command(const char *const command_to_parse, Command *const command) {
    size_t stripped_length;
    const size_t argc = find_num_args(command_to_parse, &stripped_length);
    command->argc = argc;
    // grow argv if needed
    if (argc > command->_max_argc) {
        command->argv = (const char **) realloc(command->argv, (argc + 1) * sizeof(char *));
        command->_max_argc = argc;
    }
    
    const size_t buf_len = stripped_length;
    // grow buf if needed
    if (buf_len > command->_buf_len) {
        command->_buf = (char *) realloc(command->_buf, (buf_len + 1) * sizeof(char));
        memset(command->_buf, 0, buf_len * sizeof(char));
        command->_buf_len = buf_len;
    }

    parse_command_with_buf(command_to_parse, command->argv, command->_buf);
    const char *const new_command_to_parse = preprocess_Command(command);
    if (new_command_to_parse != command->_buf) {
        parse_command(new_command_to_parse, command);
    }
}

ssize_t parse_commands(char *line, Commands *commands) {
    const ssize_t num_commands = find_num_commands(line);
//    pz(num_commands);
    if (num_commands < 0) {
        return num_commands;
    }
    
    const size_t argvc = (size_t) num_commands;
    commands->argvc = argvc;
    // grow argvv and str_commands if needed
    if (argvc > commands->_max_argvc) {
        commands->argvv = realloc(commands->argvv, argvc * sizeof(Command));
        memset(commands->argvv + commands->_max_argvc, 0, (argvc - commands->_max_argvc) * sizeof(Command));
        commands->_str_commands = realloc(commands->_str_commands, argvc * sizeof(_StrCommand));
        commands->_max_argvc = argvc;
    }
    
    _StrCommand *const str_commands = commands->_str_commands;
    parse_commands_to_strings(line, str_commands);
    
    Command *const argvv = commands->argvv;
    for (size_t i = 0; i < argvc; ++i) {
        parse_command(str_commands[i].cmd, argvv + i);
        argvv[i].post_delimiter = str_commands[i].delimiter;
        argvv[i].pre_delimiter = i == 0 ? "" : str_commands[i - 1].delimiter;
    }
    
    return 0;
}

/**
 * Free the fields of a StrCommand.
 *
 * @param [out] command the StrCommand to free
 */
static void free_StrCommand(_StrCommand *const command) {
    free((char *) command->cmd);
    command->cmd = NULL;
    command->delimiter = NULL;
    
    // same delimeter freed in free_Command
}

/**
 * Free the fields of a Command.
 *
 * @param [out] command the Command to free
 */
static void free_Command(Command *const command) {
    free((char *) command->post_delimiter);
    command->post_delimiter = NULL;
    command->pre_delimiter = NULL;
    
    command->_buf_len = 0;
    free(command->_buf);
    command->_buf = NULL;
    
    const size_t argc = command->argc;
    command->argc = 0;
    command->_max_argc = 0;
    
    char **argv = (char **) command->argv;
    for (size_t i = 0; i < argc; ++i) {
        free(argv[i]);
    }
    
    free(argv);
    command->argv = NULL;
}

void free_Commands(Commands *commands) {
    // reset max_argvc and argvc
    const size_t argvc = commands->_max_argvc;
    commands->_max_argvc = 0;
    commands->argvc = 0;
    _StrCommand *const str_commands = commands->_str_commands;
    Command *const argvv = commands->argvv;
    for (size_t i = 0; i < argvc; ++i) {
        free_StrCommand(str_commands + i);
        free_Command(argvv + i);
    }
    
    // free vectors: argvv and _str_commands
    free(str_commands);
    commands->_str_commands = NULL;
    free(argvv);
    commands->argvv = NULL;
}

char *Command_to_string(const Command *const command) {
    return str_join_with_ends(", ", command->argv, command->argc, "[", "]");
}

char *Commands_to_string(const Commands *const commands) {
    using_argvv_cc();
    if (argvc == 0) {
        return str_copy("[]");
    }
    const char **const args_list = (const char **) malloc(argvc * sizeof(char *));
    for (size_t i = 0; i < argvc; ++i) {
        const char *const command_str = Command_to_string(argvv + i);
        if (i == argvc - 1) {
            args_list[i] = command_str;
            continue;
        }
        const char *const parts[] = {command_str, argvv[i].post_delimiter};
        args_list[i] = str_join(", ", parts, arraylen(parts));
        free((char *) command_str);
    }
    const char *const s = str_join_with_ends(", ", args_list, argvc, "[", "]");
    for (size_t i = 0; i < argvc; ++i) {
        free((char *) args_list[i]);
    }
    free(args_list);
    return (char *) s;
}

void print_Command(const Command *const command) {
    const char *const s = Command_to_string(command);
    printf("%s\n", s);
    free((char *) s);
}

void print_Commands(const Commands *const commands) {
    const char *const s = Commands_to_string(commands);
    printf("%s\n", s);
    free((char *) s);
}