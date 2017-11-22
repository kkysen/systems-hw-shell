//
// Created by kkyse on 11/8/2017.
//

#ifndef SYSTEMS_SIGACTION_H
#define SYSTEMS_SIGACTION_H

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 201711L
#endif

#include <signal.h>
#include <sys/signal.h>

#ifndef SA_ONSTACK
// Call signal handler on alternate signal stack provided by sigaltstack(2).
    #define SA_ONSTACK   0x20000000
#endif

//int sigaction(int signum, const struct sigaction *action, struct sigaction *old_action);

//int sigemptyset(sigset_t *set);

void add_sigaction(int signal, struct sigaction sig_action);

const int SIGNALS[] = {SIGHUP, SIGINT, SIGQUIT, SIGILL, SIGTRAP, SIGABRT, SIGIOT, SIGEMT, SIGFPE, SIGKILL, SIGBUS,
                       SIGSEGV, SIGSYS, SIGPIPE, SIGALRM, SIGTERM, SIGURG, SIGSTOP, SIGTSTP, SIGCONT, SIGCHLD, SIGCLD,
                       SIGTTIN, SIGTTOU, SIGIO, SIGPOLL, SIGXCPU, SIGXFSZ, SIGVTALRM, SIGPROF, SIGWINCH, SIGLOST,
                       SIGPWR, SIGUSR1, SIGUSR2};

#endif //SYSTEMS_SIGACTION_H
