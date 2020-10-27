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



// char **build_command(char* input_line) {

//     char *command;
//     char **command_array[MAX_ARGS + 6];
//     int arg_num = 0;
    
//     //struct command *curr_command = malloc(sizeof(struct command));
//     //char *save_ptr;

//     command = strtok(input_line, " ");

//     while (command != NULL) {
//         *command_array[arg_num] = command;
//         arg_num++;
//         command = strtok(NULL, " ");
        
//     }

//     return command_array;
// }

// take below code from:
// https://github.com/brenns10/lsh/blob/407938170e8b40d231781576e05282a41634848c/src/main.c
// to at least help with troubleshooting.
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

char **lsh_split_line(char *line, char **tokens)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  //char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    // if (position >= bufsize) {
    //   bufsize += LSH_TOK_BUFSIZE;
    //   tokens = realloc(tokens, bufsize * sizeof(char*));
    //   if (!tokens) {
    //     fprintf(stderr, "lsh: allocation error\n");
    //     exit(EXIT_FAILURE);
    //   }
    // }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

