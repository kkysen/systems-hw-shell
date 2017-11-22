//
// Created by kkyse on 11/9/2017.
//

#ifndef SYSTEMS_SPLIT_H
#define SYSTEMS_SPLIT_H

#include <stdlib.h>
#include <stdbool.h>

extern char *args;
extern size_t args_len;

extern const char **argv;
extern size_t argc;

extern bool free_each_arg;

size_t stripped_length(const char *s, size_t *num_tokens);

char *split_r(const char *s, char *buf, const char **tokens);

const char **split(const char *line);

const char **copy_split_str(const char **split_string_ptr, size_t string_length,
                            const char *const *tokens, size_t num_tokens);

const char **copy_argv(const char **args_copy);

const char **set_argv(const char **new_argv, size_t new_argc);

void free_argv();

void print_argv();

#endif //SYSTEMS_SPLIT_H
