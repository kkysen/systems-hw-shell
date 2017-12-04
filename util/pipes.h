//
// Created by kkyse on 11/29/2017.
//

#ifndef SHELL_PIPING_H
#define SHELL_PIPING_H

typedef union pipe {
    int fds[2];
    struct {
        int read;
        int write;
    };
} Pipe;

#endif //SHELL_PIPING_H
