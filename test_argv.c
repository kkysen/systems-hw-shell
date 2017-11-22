//
// Created by kkyse on 11/9/2017.
//

#include <stdio.h>

#include "util.h"
#include "argv.h"
#include "stacktrace.h"

int main() {
    set_stack_trace_signal_handler();
    
    const char *const s = "hello world           by \t\t  Khyber Sen";
    printf("parsing \"%s\"\n", s);
    size_t num_tokens;
    size_t len = stripped_length(s, &num_tokens);
    printf("%zu == %u\n%zu == %u\n", len, 25, num_tokens, 5);
    
    
    split("hello world           by \t\t  Khyber Sen");
    printf("argc = %zu\n", argc);
    for (uint i = 0; i < argc; ++i) {
        printf("\"%s\"\n", argv[i]);
    }
}