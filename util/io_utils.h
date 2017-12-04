//
// Created by kkyse on 11/26/2017.
//

#ifndef SHELL_IO_UTILS_H
#define SHELL_IO_UTILS_H

#include <stdlib.h>
#include <sys/types.h>

/**
 * Get the length of a file.
 *
 * @param fd file descriptor of file
 * @return length of the file
 */
off_t file_length(int fd);

#endif //SHELL_IO_UTILS_H
