//
// Created by kkyse on 12/4/2017.
//

#include <stdio.h>

int main() {
    for (size_t i = 0;; ++i) {
        const int ic = getchar();
        const char c = (char) ic;
        if (c == EOF) {
            return 0;
        }
        printf("[%zu] '%c' ('%d')\n", i, c, ic);
    }
}