// initial version of parsing code below taken from:
// https://github.com/brenns10/lsh/blob/407938170e8b40d231781576e05282a41634848c/src/main.c
// to at least help with troubleshooting. Made major modifications to get current form.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "command.h"


char* get_input(void) {
    char *curr_line = NULL;
    size_t len = 0;
    ssize_t nread;  
    nread = getline(&curr_line, &len, stdin);
    return curr_line;
}



#define LSH_TOK_DELIM " \t\r\n\a"

char **lsh_split_line(char *line, char **command, char **args, int *n_args)
{
    int index = 0;
    char *token;

    token = strtok(line, LSH_TOK_DELIM);
    *command = token;

    while (token != NULL) {
        token = strtok(NULL, LSH_TOK_DELIM);
        args[index] = token;
        index++;    
  }
    *n_args = index - 1;
    //args[index] = NULL;
}



