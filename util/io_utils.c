//
// Created by kkyse on 11/26/2017.
//

#include "io_utils.h"

#include <sys/stat.h>

off_t file_length(const int fd) {
    struct stat stats;
    if (fstat(fd, &stats) == -1) {
        return -1;
    }
    return stats.st_size;
}