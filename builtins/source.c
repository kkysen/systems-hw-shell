//
// Created by kkyse on 11/26/2017.
//

#include "source.h"

#include <fcntl.h>

#include "../run_commands.h"
#include "../shell_utils.h"
#include "../shell_io.h"

int source_file(const char *const filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
//        perror("fopen");
        fprintf(stderr, "-shell: source: couldn't open %s\n", filename);
        return einval("fopen error");
    }
    
    shell_input = file;
    
    loop(&all_commands, false);
    
    fclose(file);
    shell_input = stdin;
    
    return EXIT_SUCCESS;
}

int source_cmd(const Command *const command) {
    if (command->argc != 2) {
        return einval("source takes 1 argument, a file\n");
    }
    const char *const filename = command->argv[1];
    return source_file(filename);
}