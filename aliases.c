//
// Created by kkyse on 11/10/2017.
//

#include "aliases.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util/str_utils.h"
#include "util/utils.h"

typedef struct alias {
    const size_t index;
    const char *const name;
    const char *const value;
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

/**
 * Increase the capacity of the aliases by \param step
 * and realloc them with the new capacity.
 *
 * @param [in] step how much to increase the aliases capacity by
 */
static void realloc_aliases(const ssize_t step) {
    aliases_capacity += step;
    aliases = realloc((Alias *) aliases, aliases_capacity * sizeof(Alias));
}

/**
 * Grow the aliases by 1 and realloc if over aliases_capacity.
 */
static void grow_aliases() {
    ++num_aliases;
    if (num_aliases > aliases_capacity) {
        realloc_aliases(aliases_capacity_step);
    }
}

/**
 * Shrink the aliases (by reallocing)
 * if aliases_capacity is more than aliases_capacity_step greater than num_aliases.
 */
static void shrink_aliases() {
    if (num_aliases + aliases_capacity_step > aliases_capacity) {
        realloc_aliases(-aliases_capacity_step);
    }
}

/**
 * Set the name and value of the Alias at index \param index
 * to \param name and \param value.
 *
 * @param [in] index the index of the Alias
 * @param [in] name the new name of the Alias
 * @param [in] value the new value of the Alias
 */
void set_Alias(const size_t index, const char *const name, const char *const value) {
    const Alias stack_alias = {
            .index = index,
            .name = str_copy(name),
            .value = str_copy(value),
    };
    memcpy((Alias *) aliases + index, &stack_alias, sizeof(Alias));
}

/**
 * Free the fields of an Alias.
 *
 * @param [in, out] alias the Alias to free
 */
static void free_Alias(const Alias *const alias) {
    free((char *) alias->name);
    free((char *) alias->value);
}

void free_aliases() {
    for (size_t i = 0; i < num_aliases; ++i) {
        free_Alias(aliases + i);
    }
    free((Alias *) aliases);
}

void add_alias(const char *const name, const char *const value) {
    if (strcmp(name, value) == 0) {
        return; // will cause infinite loop
    }
    if (get_alias_value(name)) {
        remove_alias_by_name(name);
    }
    grow_aliases();
    set_Alias(num_aliases - 1, name, value);
}

#define get_index_by(field, value) \
    for (size_t i = 0; i < num_aliases; ++i) { \
        if (strcmp(value, aliases[i].field) == 0) { \
            return i; \
        } \
    } \
    return -1;

/**
 * Get the index of an Alias by \param name.
 *
 * @param [in] name the name of the Alias
 * @return the index of the Alias, or -1 if it doesn't exist
 */
static ssize_t get_Alias_index_by_name(const char *const name) {
    get_index_by(name, name);
}

/**
 * Get the index of an Alias by \param value.
 *
 * @param [in] value the value of the Alias
 * @return the index of the Alias, or -1 if it doesn't exist
 */
static ssize_t get_Alias_index_by_value(const char *const value) {
    get_index_by(value, value);
}

/**
 * Remove the Alias at index \param i.
 *
 * @param [in] i the index of the Alias to remove
 */
static void remove_Alias_at_index(const size_t i) {
    --num_aliases;
    free_Alias(aliases + i);
    memmove((Alias *) aliases + i, aliases + i + 1, (num_aliases - i) * sizeof(Alias));
    shrink_aliases();
}

#define remove_alias_func(field) \
bool remove_alias_by_##field(const char *const field) { \
    const ssize_t i = get_Alias_index_by_##field(field); \
    if (i == -1) { \
        return false; \
    } \
    remove_Alias_at_index((size_t) i); \
    return true; \
}

remove_alias_func(name);

remove_alias_func(value);

#define get_alias_func(get_field, by_field) \
const char *get_alias_##get_field(const char *const by_field) { \
    const ssize_t i = get_Alias_index_by_##by_field(by_field); \
    if (i == -1) { \
        return NULL; \
    } \
    return aliases[i].get_field; \
}

// get_alias_value (by name)
get_alias_func(value, name)

// get_alias_name (by value)
get_alias_func(name, value)

void print_alias(const char *const name, const char *const value) {
    printf("alias %s='%s'\n", name, value);
}

void for_each_alias(void (*const callback)(const char *name, const char *value)) {
    for (size_t i = 0; i < num_aliases; ++i) {
        callback(aliases[i].name, aliases[i].value);
    }
}