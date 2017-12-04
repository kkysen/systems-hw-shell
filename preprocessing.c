//
// Created by kkyse on 11/26/2017.
//

#include "preprocessing.h"

#include <stdio.h>
#include <string.h>

#include "util/utils.h"
#include "util/str_utils.h"
#include "aliases.h"

const char *preprocess_str_command(const char *const command, const size_t length) {
    return command; // does nothing for now
}

/**
 * Preprocess a \param command by substituting aliases.
 *
 * @param [in] command the command to preprocess
 * @return the new string with the substituted aliases,
 *      or the same backing string if no aliases found
 */
static const char *preprocess_aliases(const Command *const command) {
    const size_t argc = command->argc;
    const char **const argv = command->argv;
    bool expanded_aliases = false;
    for (size_t i = 0; i < argc; ++i) {
        const char *const arg = argv[i];
//        print_Command(command);
        const char *const alias = get_alias_value(arg);
        if (alias) {
            expanded_aliases = true;
            argv[i] = alias;
        }
    }
    
    if (!expanded_aliases) {
        return command->_buf;
    }
    
    // join argv for new str_command to re-parse
    return str_join(" ", argv, argc);
}

typedef const char *(*CommandPreprocessor)(const Command *const command);

static const CommandPreprocessor command_preprocessors[] = {
        preprocess_aliases,
};

static size_t command_preprocess_index = 0;

/**
 * Each CommandPreprocessor must be called successively in #preprocess_Command(),
 * so the index of the CommandPreprocessor must be maintained in-between calls.
 * When all the CommandPreprocessors are done, then cycle back to the first one.
 *
 * @return the index of the next CommandPreprocessor
 */
static size_t next_command_preprocessor_index() {
    if (++command_preprocess_index == arraylen(command_preprocessors)) {
        command_preprocess_index = 0;
    }
    return command_preprocess_index;
}

const char *preprocess_Command(const Command *command) {
    return command_preprocessors[next_command_preprocessor_index()](command);
}

/**
 * Preprocess a single argument by expanding '~'s to $HOME.
 *
 * @param [in] arg the argument to preprocess
 * @return the processed argument
 */
static const char *preprocess_tilde(const char *const arg) {
    if (arg[0] != '~') {
        return arg;
    }
    
    if (arg[1] == 0) {
        // if only lone '~', then just replace with HOME
        return str_copy(HOME);
    } else {
        return str_replace(arg, "~", HOME, 0, (size_t) -1);
    }
}

/**
 * Preprocess a single argument by expanding environment variables in the form $NAME.
 *
 * @param [in] arg the argument to preprocess
 * @return the processed argument
 */
static const char *preprocess_env_vars(const char *const arg) {
    if (arg[0] != '$') {
        return arg;
    }
    const char *const env_var = getenv(arg + 1);
    if (!env_var) {
        fprintf(stderr, "%s is not an environment variable\n", arg);
        return arg;
    }
    return str_copy(env_var);
}

typedef const char *(*ArgPreprocessor)(const char *const arg);

static const ArgPreprocessor arg_preprocessors[] = {
        preprocess_tilde,
        preprocess_env_vars,
};

/**
 * Preprocess a single argument.
 * If no proprocessing is done, return the same arg.
 *
 * @param [in] arg the argument to preprocess
 * @return the processed argument
 */
static const char *preprocess_arg(const char *arg) {
//    #pragma unroll
    for (size_t i = 0; i < arraylen(arg_preprocessors); ++i) {
        arg = arg_preprocessors[i](arg);
    }
    return arg;
}

void preprocess_command(const Command *const command) {
    const size_t argc = command->argc;
    const char **const argv = command->argv;
    for (size_t i = 0; i < argc; ++i) {
        const char *const arg = argv[i];
        const char *const new_arg = preprocess_arg(arg);
        if (arg != new_arg) {
            free((char *) arg);
        }
        argv[i] = new_arg;
    }
}