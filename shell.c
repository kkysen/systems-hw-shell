//
// Created by kkyse on 9/24/2017.
//

#include "shell.h"

#include <stdlib.h>

#include "run_commands.h"
#include "util/utils.h"
#include "util/stacktrace.h"
#include "aliases.h"
#include "builtins/cd.h"
#include "shell_io.h"
#include "builtins/source.h"


/**
 * Initializes the shell.
 */
static void init_shell() {
    #ifdef __linux__
    set_stack_trace_signal_handler();
    #endif
    setup_tty();
    init_home();
    set_current_dir();
    source_file("init.sh");
}

/**
 * Free all of the shell's resources.
 */
static void free_shell() {
    free(line);
    free_aliases();
}

void run_shell() {
    init_shell();
    loop(&all_commands, true);
    free_shell();
}

#ifndef MAIN
    #define MAIN

/**
 * Start the program and run the shell.
 *
 * @return 0 if no error
 */
int main() {
    run_shell();
    return 0;
}

#endif