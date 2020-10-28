#include <stdio.h>
#include <stdbool.h>

#define MAX_CLL 2048
#define MAX_ARGS 512



#ifndef THERE_IS_A_COMMAND
#define THERE_IS_A_COMMAND

struct command
{
    int arg_count;
    char **args;
    char *input_redirect;
    char *output_redirect;
    bool background;
};

char* get_input_line(void);
char** parse_input_line(char *curr_line, int *n_inputs);
bool is_redirect_out(char* input);
bool is_redirect_in(char* input);
bool is_bg_command(char** inputs, int* n_inputs);
struct command *build_prelim_command(char** inputs, int *n_inputs);
struct command *get_prelim_command(void);
void expand_var(struct command* curr_command, char* old_str, char* new_str);
//char **lsh_split_line(char *line, char **command, char **args, int *n_args);

#endif