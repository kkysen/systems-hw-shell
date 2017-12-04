//
// Created by kkyse on 11/26/2017.
//

#include "unalias.h"

#include "../shell_utils.h"
#include "../aliases.h"

int unalias_cmd(const Command *const command) {
    if (command->argc < 2) {
        return einval("unalias requires an argument, the alias to remove\n");
    }
    const char *const value = command->argv[1];
    if (!remove_alias_by_value(value)) {
        printf("-"SHELL": unalias: %s: not found\n", get_alias_name(value));
    }
    return EXIT_SUCCESS;
}