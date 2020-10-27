#include <stdio.h>
#include <stdbool.h>
#define MAX_CLL 2048
#define MAX_ARGS 512



#ifndef THERE_IS_A_COMMAND
#define THERE_IS_A_COMMAND

struct command
{
    char *command;
    int arg_count;
    char **args;
    char *input_redirect;
    char *output_redirect;
    bool background;
};

char* get_input(void);
// struct command *build_command(char *input_line);
//char **build_command(char *input_line);
char **lsh_split_line(char *line, char **command, char **args, int *n_args);

#endif