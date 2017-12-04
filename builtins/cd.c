//
// Created by kkyse on 11/11/2017.
//

#include "cd.h"

#include <unistd.h>

#include "../shell_utils.h"

void set_current_dir() {
    if (!getcwd(CURRENT_DIR, sizeof(CURRENT_DIR) - 1)) {
        perror("getcwd");
    }
}

int cd_cmd(const Command *const command) {
    if (command->argc < 2) {
        return einval("cd requires an argument, a directory\n");
    }
    if (command->argc > 2) {
        return einval("cd: too many arguments (cd takes 1 argument)\n");
    }
    
    const char *const dir = command->argv[1];
    if (chdir(dir) != 0) {
        shell_perror();
    }
    set_current_dir();
    return EXIT_SUCCESS;
}