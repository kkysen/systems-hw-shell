//
// Created by kkyse on 11/9/2017.
//

#include "argv.h"

#include <string.h>
#include <stdio.h>

#include "util.h"

char *args = NULL;
size_t args_len = 0;

const char **argv = NULL;
size_t argc = 0;
size_t argc_max = 0;

bool free_each_arg = false;

#define is_newline(c) ((c) == '\n' || (c) == '\r')
#define is_whitespace(c) ((c) == ' ' || (c) == '\t' || is_newline(c))

/*
 * Returns length of s if all whitespace sections were replaced by a single space.
 * num_tokens will be set to number of tokens separated by whitespace.
 */
size_t stripped_length(const char *s, size_t *const num_tokens) {
    *num_tokens = 0;
    if (is_newline(s[0]) && (s[1] == 0 || (is_newline(s[1]) && s[2] == 0))) {
        // no input, only newline
        return 0;
    }
    char c;
    size_t length = 0;
    for (--s; (c = *++s); ++length) {
//        printf("%c\n", c);
        if (is_whitespace(c)) {
            while ((c = *++s) && is_whitespace(c));
            if (!c) {
                break;
            }
            --s;
            ++*num_tokens;
        }
    }
    ++*num_tokens;
    return length;
}

/*
 * Splits string s by whitespace (' ', '\t')
 * into multiple strings separated by nulls in buf.
 * Returns the end of buf (after the trailing null).
 */
char *split_r(const char *s, char *buf, const char **tokens) {
    char c;
    *tokens++ = buf;
    for (--s, --buf; (c = *++s);) {
        if (is_whitespace(c)) {
            while ((c = *++s) && is_whitespace(c));
            printf("%c\n", c);
            if (!c) {
                break;
            }
            --s;
            c = 0;
        }
        *++buf = c;
        if (!c) {
            *tokens++ = buf + 1;
//            printf("'%c'\n", buf[-1]);
        }
    }
    *++buf = 0;
    *tokens = NULL;
    return buf;
}

void grow_argv(const size_t new_argc) {
    if (new_argc > argc_max) {
        argv = (const char **) realloc(argv, (new_argc + 1) * sizeof(*argv));
        argc_max = new_argc;
    }
    argc = new_argc;
}

const char **split(const char *const line) {
    size_t num_tokens;
    const size_t stripped_len = stripped_length(line, &num_tokens);
    argc = num_tokens;
    printf("argc = %zu, args_len = %zu\n", argc, stripped_len);
    if (argc == 0) {
        return NULL;
    }
    
    if (stripped_len > args_len) {
        args = (char *) realloc(args, (stripped_len + 1) * sizeof(*args));
        args_len = stripped_len;
    }
    
    grow_argv(num_tokens);
    
    split_r(line, args, argv);
    return argv;
}

/*
 * Copies previously split_string into malloc'd new split string and new tokens.
 * The new tokens are returned and the new split string is returned through the split_string_ptr.
 */
const char **copy_split_str(const char **const split_string_ptr, const size_t string_length,
                            const char *const *const tokens, const size_t num_tokens) {
    const char *const old_str = *split_string_ptr;
    const size_t str_size = (string_length + 1) * sizeof(*old_str);
    char *const new_str = (char *) malloc(str_size);
    memcpy(new_str, old_str, str_size);
    *split_string_ptr = new_str;
    
    const size_t tokens_size = (num_tokens + 1) * sizeof(*tokens);
    const char **const new_tokens = (const char **) malloc(tokens_size);
    const size_t token_offset = new_str - old_str;
    for (size_t i = 0; i < num_tokens; ++i) {
        new_tokens[i] = tokens[i] + token_offset;
    }
    
    return new_tokens;
}

const char **copy_argv(const char **const args_copy) {
    *args_copy = args;
    return copy_split_str(args_copy, args_len, argv, argc);
}

const char **set_argv(const char **const new_argv, const size_t new_argc) {
    grow_argv(new_argc);
    memcpy(argv, new_argv, argc * sizeof(*argv));
    return argv;
}

void switch_to_individual_argv() {
    if (free_each_arg) {
        return; // already done
    }
    free_each_arg = true;
    for (size_t i = 0; i < argc; ++i) {
        argv[i] = copy_str(argv[i]);
    }
}

void free_argv() {
    free(args);
    if (free_each_arg) {
        for (size_t i = 0; i < argc; ++i) {
            free((char *) argv[i]);
        }
    }
    free(argv);
}

void print_argv() {
    printf("argv: [");
    for (uint i = 0; i < argc; ++i) {
        printf("%s", argv[i]);
        if (i + 1 != argc) {
            printf(", ");
        }
    }
    printf("]\n");
}