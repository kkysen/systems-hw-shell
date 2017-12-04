//
// Created by kkyse on 11/26/2017.
//

#ifndef SHELL_SHELL_IO_H
#define SHELL_SHELL_IO_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

extern const bool RAW_TTY;

extern FILE *shell_input;

/**
 * Sets up the terminal to be in either raw or cooked mode.
 *
 * If RAW_TTY is true, then it will be in raw mode.
 * Otherwise, it will be in cooked mode.
 *
 * Cooked mode is simpler, but the terminal blocks until a newline is entered,
 * so the shell has less control.
 *
 * Raw mode offers more control, reading each character as it's typed,
 * but it's more work to process everything accurately.
 */
void setup_tty();

extern char *line;
extern size_t line_len;

/**
 * Reads a line from stdin,
 * calling the respective raw and cooked version based on RAW_TTY.
 *
 * @return the line read from stdin
 */
char *read_line();

#endif //SHELL_SHELL_IO_H
