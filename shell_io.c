//
// Created by kkyse on 11/26/2017.
//

#include "shell_io.h"

#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include "util/utils.h"
#include "shell_utils.h"
#include "argvv.h"

const bool RAW_TTY = false;

char *line = NULL;
size_t line_len = 0;

FILE *shell_input = NULL;

void setup_tty() {
    shell_input = stdin;
    
    const char *cmd;
    if (RAW_TTY) {
        // turn cooked mode off so no line-buffering
        // read each char individually without delay
        cmd = "/bin/stty raw";
    } else {
        cmd = "/bin/stty cooked";
    }
    if (system(cmd) != 0) {
        perror("system");
        return;
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
//    p("set termios");
    return;
    
    termios_failed:
    perror("termios");
}

/**
 * Reads a line from stdin, assuming the terminal is in cooked mode.
 *
 * @return the line read from stdin
 */
static char *read_line_cooked() {
    size_t size;
    if (getline(&line, &size, shell_input) == -1) {
        return NULL;
    }
    return line;
}

/**
 * Reads a line from stdin, assuming the terminal is in raw mode.
 *
 * Ctrl C raises SIGINT
 * Ctrl Z raises SIGSTOP
 * Backspaces removed a character
 * '\n' and '\r' terminate the line
 *
 * @return the line read from stdin
 */
static char *read_line_raw() {
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
                print_Commands(&all_commands);
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