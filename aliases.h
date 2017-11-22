//
// Created by kkyse on 11/10/2017.
//

#ifndef SYSTEMS_ALIASES_H
#define SYSTEMS_ALIASES_H

#include <stdbool.h>

void free_aliases();

void add_alias(const char *name, const char *value);

/*
 * True if alias removed
 */
bool remove_alias(const char *name);

const char *get_alias(const char *name);

void print_alias(const char *name, const char *value);

void for_each_alias(void (*callback)(const char *name, const char *value));

/*
 * expand aliases in argv, changing it as necessary
 */
void expand_aliases();

#endif //SYSTEMS_ALIASES_H