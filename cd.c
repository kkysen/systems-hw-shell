//
// Created by kkyse on 11/11/2017.
//

#include "cd.h"

#include <unistd.h>

#include "shell_util.h"
#include "argv.h"

char CURRENT_DIR[PATH_MAX];

void set_current_dir() {
    getcwd(CURRENT_DIR, sizeof(CURRENT_DIR) - 1);
}

int cd_cmd() {
    if (argc < 2) {
        return einval("cd requires an argument, a directory\n");
    }
    const char *const dir = argv[1];
    if (chdir(dir) != 0) {
        shell_error();
    }
    set_current_dir();
    return EXIT_SUCCESS;
}