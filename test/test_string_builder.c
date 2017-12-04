//
// Created by kkyse on 12/3/2017.
//

#include "../util/stacktrace.h"
#include "../util/string_builder.h"

int main() {
    set_stack_trace_signal_handler();
    
    StringBuilder *const sb = StringBuilder_new(0);
    for (int i = 0; i < 100; ++i) {
        StringBuilder_append_string(sb, "hello, world");
    }
    StringBuilder_free(sb);
}