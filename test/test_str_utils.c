//
// Created by kkyse on 11/26/2017.
//

#include <stdio.h>
#include <stdlib.h>

#include "../util/stacktrace.h"
#include "../util/utils.h"
#include "../util/str_utils.h"

int main() {
    //set_stack_trace_signal_handler();
    
    const char *const strings[] = {
            "hello",
            "world",
            "what",
            "the",
            "fuck",
    };
    
    const char *const s = str_join_with_ends(", ", strings, arraylen(strings), "[", "]");
    printf("%s\n", s);
    free((char *) s);
    
    
    char *const s2 = str_copy(" pwd\n");
    const char *const s3 = strip(s2);
    printf("'%s'\n", s3);
    free(s2);
    return 0;
}