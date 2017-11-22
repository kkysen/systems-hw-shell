//
// Created by kkyse on 9/24/2017.
//

#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <stdbool.h>
#include <termios.h>

#include "util.h"
#include "stacktrace.h"
#include "argv.h"
#include "shell_util.h"
#include "builtins.h"
#include "aliases.h"
#include "cd.h"

const bool RAW_TTY = false;

void setup_tty() {
    if (RAW_TTY) {
        // turn cooked mode off so no line-buffering
        // read each char individually without delay
        system("/bin/stty raw");
    } else {
        system("/bin/stty cooked");
    }
    
    struct termios tty;
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        goto termios_failed;
    }
    tty.c_lflag |= ICANON | ECHO | ISIG;
    tty.c_iflag |= IXON | IUTF8;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        goto termios_failed;
    }
    printf("set termios\n");
    return;
    
    termios_failed:
    perror("termios");
}

static const char *HOME;

void init_shell() {
    set_stack_trace_signal_handler();
    setup_tty();
    HOME = getenv("HOME");
    set_current_dir();
}

static char *line = NULL;
static size_t line_len = 0;

char *read_line_cooked() {
    size_t size;
    getline(&line, &size, stdin);
    return line;
}

char *read_line_raw() {
    int c;
    uint i = 0;
    while ((c = getchar()) != EOF) {
        switch (c) {
            case 3: // Ctrl C
                raise(SIGINT);
                return NULL;
            case 26: // Ctrl Z
                raise(SIGSTOP);
                return NULL;
            case 127: // Backspace
                if (i > 0) {
                    --i;
                    printf("\b");
                }
                break;
            case 18:  // Ctrl R
                print_argv();
                return NULL;
            case '\n':
            case '\r':
                goto after_loop;
            default:
//                printf("input char: %d\n", c);
                if (i == line_len) {
                    line_len += 1024;
                    line = (char *) realloc(line, (line_len + 1) * sizeof(*line));
                }
                line[i++] = (char) c;
        }
    }
    after_loop:
    if (i == 0) {
        return NULL;
    }
    line[i] = 0;
//    printf("input: \"%s\"\n", line);
    return line;
}

char *read_line() {
    if (RAW_TTY) {
        return read_line_raw();
    } else {
        return read_line_cooked();
    }
}

void process_argv() {
    for (uint i = 0; i < argc; ++i) {
        const char *const arg = argv[i];
        if (arg[0] == '~') {
            if (arg[1] == 0) {
                // if only lone '~', then no new string needed
                argv[i] = HOME;
            } else {
                // must create new string
                if (!free_each_arg) {
                    free_each_arg = true;
                    argv = copy_argv()
                }
            }
        }
        if (arg[0] == '~' && arg[1] == 0) {
            // if lone '~' arg, expand to home path
            argv[i] = HOME;
        }
    }
}

int launch() {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(argv[0], (char **) argv);
        // must be an error if execvp returns
        shell_error();
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        shell_error();
    } else {
        int status;
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return EXIT_SUCCESS;
}

int execute() {
    if (argc == 0) {
        return EXIT_SUCCESS;
    }
    const char *const name = *argv;
//    #pragma unroll
    for (uint i = 0; i < num_builtins; ++i) {
        Builtin builtin = builtins[i];
        if (strcmp(name, builtin.name) == 0) {
            return builtin.call();
        }
    }
    return launch();
}

void loop() {
    int status;
    do {
        printf("%s$ ", CURRENT_DIR);
        char *const line = read_line();
        if (!line) {
            printf("\n");
            continue;
        }
        split(line);
        process_argv();

//        #if (true)
        printf("read line \"%s\"\n", line);
        print_argv();
//        #endif
        
        status = execute();
    } while (status == EXIT_SUCCESS);
}

void free_shell() {
    free(line);
    free_argv();
    free_aliases();
}

void run_shell() {
    init_shell();
    loop();
    free_shell();
}

#ifndef MAIN
    #define MAIN

int main(const int argc, const char *const *const argv) {
    run_shell();
}

#endif