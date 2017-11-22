//
// Created by kkyse on 11/9/2017.
//

#include "builtins.h"

#include <string.h>
#include <stdio.h>

#include "shell_util.h"
#include "argv.h"
#include "aliases.h"
#include "cd.h"

int exit_cmd() {
    return EXIT_FAILURE; // terminate while(status) loop
}

int alias_cmd() {
    if (argc < 2) {
        // "alias" should print out all aliases
        for_each_alias(print_alias);
        return EXIT_SUCCESS;
    }
    
    char *value = (char *) argv[1];
    const char *const name = strsep(&value, "=");
    if (!value) {
        // "alias <name>" should print out alias to <name>
        printf("-"SHELL": alias: %s: ", name);
        const char *assigned_value = get_alias(name);
        printf("%s\n", assigned_value ? assigned_value : "not found");
        return EXIT_SUCCESS;
    }
    
    // "alias <name>=<value> should assign <value> to <name> in aliases
    add_alias(name, value);
    return EXIT_SUCCESS;
}

int unalias_cmd() {
    if (argc < 2) {
        return einval("unalias requires an argument, the alias to remove\n");
    }
    const char *const name = argv[1];
    if (!remove_alias(name)) {
        printf("-"SHELL": unalias: %s: not found\n", name);
    }
    return EXIT_SUCCESS;
}

const Builtin builtins[] = {
        {"cd", &cd_cmd},
        {"exit", &exit_cmd},
        {"alias", &alias_cmd},
        {"unalias", &unalias_cmd},
};

const uint num_builtins = arraylen(builtins);