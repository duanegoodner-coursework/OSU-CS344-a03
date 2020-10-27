#include <stdio.h>
#include <stdlib.h>
#include "command.h"

#define C_PROMPT ":"


int main(void) {
    char input_buf[MAX_CLL];
    char *args[MAX_ARGS];
    int run_flag = 1;

    while (run_flag) {
        printf(C_PROMPT);
        fflush(stdout);
        char* input_str = get_input();
        lsh_split_line(input_str, args);
        printf("\n\n pause here to reflect\n\n");
    }

    return 0;
}




