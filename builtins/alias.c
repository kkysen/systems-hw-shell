//
// Created by kkyse on 11/26/2017.
//

#include "alias.h"

#include <stdio.h>
#include <string.h>

#include "../aliases.h"
#include "../shell_utils.h"
#include "../util/str_utils.h"

int alias_cmd(const Command *const command) {
    if (command->argc < 2) {
        // "alias" should print out all aliases
        for_each_alias(print_alias);
        return EXIT_SUCCESS;
    }
    
    char *value = (char *) command->argv[1];
    const char *const name = strsep(&value, "=");
    if (!value) {
        // "alias <name>" should print out alias to <name>
        printf("-"SHELL": alias: %s: ", name);
        const char *assigned_value = get_alias_value(name);
        printf("%s\n", assigned_value ? assigned_value : "not found");
        return EXIT_SUCCESS;
    }
    
    // "alias <name>=<value> should assign <value> to <name> in aliases
    // <value> may be quoted
    add_alias(name, unquote(value));
    return EXIT_SUCCESS;
}