//
// Created by kkyse on 11/10/2017.
//

#ifndef SYSTEMS_ALIASES_H
#define SYSTEMS_ALIASES_H

#include <stdbool.h>

/**
 * Free all the aliases.
 */
void free_aliases();

/**
 * Add an alias with the name \param name and the value \param value.
 *
 * @param [in] name the name of the new alias
 * @param [in] value the value of the new alias
 */
void add_alias(const char *name, const char *value);

/**
 * Remove the alias with the name \param name.
 *
 * @param [in] name the name of the alias to remove
 * @return true if the alias was removed (false if it didn't exist)
 */
bool remove_alias_by_name(const char *name);

/**
 * Remove the alias with the value \param value.
 *
 * @param [in] value the value of the alias to remove
 * @return true if the alias was removed (false if it didn't exist)
 */
bool remove_alias_by_value(const char *value);

/**
 * Get the value of an alias with name \param name.
 *
 * @param [in] name the name of the alias
 * @return the value of the alias or NULL if that alias doesn't exist
 */
const char *get_alias_value(const char *name);

/**
 * Get the name of an alias with value \param value.
 *
 * @param [in] value the value of the alias
 * @return the name of the alias or NULL if that alias doesn't exist
 */
const char *get_alias_name(const char *value);

/**
 * Print the name and value of an Alias with correct formatting
 * (as per the alias Builtin command).
 *
 * @param [in] name the name of the alias
 * @param [in] value the value of the alias
 */
void print_alias(const char *name, const char *value);

/**
 * Run \param callback on the name and value of each Alias.
 *
 * @param [in] callback the name-value callback to run on the name and value of each Alias
 */
void for_each_alias(void (*callback)(const char *name, const char *value));

#endif //SYSTEMS_ALIASES_H