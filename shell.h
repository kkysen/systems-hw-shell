//
// Created by kkyse on 11/9/2017.
//

#ifndef SYSTEMS_SHELL_H
#define SYSTEMS_SHELL_H

#ifndef _POSIX_C_SOURCE
    #define _POSIX_C_SOURCE 201711L
#endif

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE 201711L
#endif

#ifndef _BSD_SOURCE
    #define _BSD_SOURCE 201711L
#endif

/**
 * Runs the shell forever unless "exit" is typed to terminate it or a SIGKILL, etc. is sent.
 */
void run_shell();

#endif //SYSTEMS_SHELL_H
