#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "command.h"

#define C_PROMPT ":"


int main(void) {
    // char input_buf[MAX_CLL];
    char *args[MAX_ARGS];
    char *command;
    int n_inputs;
    int run_flag = 1;
    


    while (run_flag) {
        printf(C_PROMPT);
        fflush(stdout);
       
        char* curr_line = get_input_line();
        char** inputs = parse_input(curr_line, &n_inputs);
        struct command *new_command = build_command(inputs, &n_inputs);
        printf("\n");
        //lsh_split_line(input_str, &command, args, &n_args);
    }

    return 0;
}




