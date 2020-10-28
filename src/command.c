// initial version of parsing code below taken from:
// https://github.com/brenns10/lsh/blob/407938170e8b40d231781576e05282a41634848c/src/main.c
// to at least help with troubleshooting. Made major modifications to get current form.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "command.h"



char* get_input_line(void) {
    char *curr_line = NULL;
    size_t len = 0;
    ssize_t nread;  
    nread = getline(&curr_line, &len, stdin);
    return curr_line;  
}

#define DELIMITERS " \t\r\n\a"


char** parse_input(char *curr_line, int *n_inputs) {
    int index = 0;
    char** inputs = malloc(MAX_ARGS + 7);
    char* token;

    token = strtok(curr_line, DELIMITERS);

    while (token != NULL) {
        inputs[index] = token;
        token = strtok(NULL, DELIMITERS);
        index++;
    }

    *n_inputs = index;
    inputs[index] = NULL;

    return inputs;
}

#define REDIRECT_OUT ">"
bool is_redirect_out(char* input) {
    if (strcmp(input, REDIRECT_OUT)) {
        return false;
    } else {
        return true;
    }
}

#define REDIRECT_IN "<"
bool is_redirect_in(char* input) {
    if (strcmp(input, REDIRECT_IN)) {
        return false;
    } else {
        return true;
    }
}

#define BG_FLAG "&"
bool is_bg_command(char** inputs, int *n_inputs) {
    if (strcmp(inputs[*n_inputs - 1], BG_FLAG)) {
        return false;
    } else {
        return true;
    }
}

struct command *build_command(char** inputs, int *n_inputs) {
    
    struct command *new_command = malloc(sizeof(struct command));
    int index;
    int index_limit = *n_inputs;
    int arg_count = 0;

    if (is_bg_command(inputs, n_inputs)) {
        index_limit--;
        new_command->background = true;
    } else {
        new_command->background = false;
    }

    for (index = 0; index < index_limit; index++) { //stop before final char
        if (is_redirect_in(inputs[index])) {
            new_command->input_redirect = inputs[index + 1];
            index++;
        } else if (is_redirect_out(inputs[index])) {
            new_command->output_redirect = inputs[index + 1];
            index++;
        } else {
            arg_count++;
        }
    }
    new_command->arg_count = arg_count;

    char** args = malloc(arg_count * sizeof(char*));

    for (index = 0; index < arg_count; index++) {
        args[index] = inputs[index];
    }

    new_command->args = args;

    return new_command;

}
