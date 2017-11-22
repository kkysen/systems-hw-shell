//
// Created by kkyse on 11/21/2017.
//

#include <err.h>
#include <stdlib.h>
#include "sigaction.h"

void add_sigaction(const int signal, const struct sigaction sig_action) {
    if (sigaction(signal, &sig_action, NULL) != 0) {
        err(EXIT_FAILURE, "sigaction");
    }
}
