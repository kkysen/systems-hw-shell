//
// Created by kkyse on 11/10/2017.
//

#include "aliases.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "argv.h"

typedef struct alias {
    const size_t index;
    const char *const name;
    const char *const value;
    const size_t args_len;
    const char *const args;
    const size_t argc;
    const char *const *const argv;
} Alias;

/*
 * Using a linear array for now,
 * but ideally this should be done using a HashMap,
 * but I'd have to right that myself.
 */

static const size_t aliases_capacity_step = 4;

static size_t num_aliases = 0;
static size_t aliases_capacity = 0;
static const Alias *aliases = NULL;

void realloc_aliases(const ssize_t step) {
    aliases_capacity += step;
    aliases = realloc((Alias *) aliases, aliases_capacity * sizeof(Alias));
}

void grow_aliases() {
    ++num_aliases;
    if (num_aliases > aliases_capacity) {
        realloc_aliases(aliases_capacity_step);
    }
}

void shrink_aliases() {
    if (num_aliases + aliases_capacity_step <= aliases_capacity_step) {
        realloc_aliases(-aliases_capacity_step);
    }
}

void set_Alias(const size_t index, const char *const name, const char *const value) {
    char *const name_copy = copy_str(name);
    char *const value_copy = copy_str(value);
    
    size_t argc;
    const size_t args_len = stripped_length(value_copy, &argc);
    
    char *const args = malloc((args_len + 1) * sizeof(*args));
    const char **const argv = malloc((argc + 1) * sizeof(*argv));
    split_r(value_copy, args, argv);
    
    const Alias stack_alias = {
            .index = index,
            .name = name_copy,
            .value = value_copy,
            .args_len = args_len,
            .args = args,
            .argc = argc,
            .argv = argv,
    };
    memcpy((Alias *) aliases + index, &stack_alias, sizeof(Alias));
}

void free_Alias(const Alias *const alias) {
    free((char *) alias->name);
    free((char *) alias->value);
    free((char *) alias->args);
    free((char **) alias->argv);
}

void free_aliases() {
    for (size_t i = 0; i < num_aliases; ++i) {
        free_Alias(aliases + i);
    }
    free((Alias *) aliases);
}

void add_alias(const char *const name, const char *const value) {
    grow_aliases();
    set_Alias(num_aliases - 1, name, value);
}

ssize_t get_Alias_index(const char *const name) {
    for (size_t i = 0; i < num_aliases; ++i) {
        if (strcmp(name, aliases[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

bool remove_alias(const char *const name) {
    const ssize_t i = get_Alias_index(name);
    if (i == -1) {
        return false;
    }
    --num_aliases;
    free_Alias(aliases + i);
    memmove((Alias *) aliases + i, aliases + i + 1, (num_aliases - i) * sizeof(Alias));
    shrink_aliases();
    return true;
}

const char *get_alias(const char *const name) {
    const ssize_t i = get_Alias_index(name);
    if (i == -1) {
        return NULL;
    }
    return aliases[i].name;
}

void print_alias(const char *const name, const char *const value) {
    printf("alias %s='%s'\n", name, value);
}

void for_each_alias(void (*const callback)(const char *name, const char *value)) {
    for (size_t i = 0; i < num_aliases; ++i) {
        callback(aliases[i].name, aliases[i].value);
    }
}

/*
 * expand aliases in argv, changing it as necessary
 */
void expand_aliases() {

}