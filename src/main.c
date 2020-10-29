#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "command.h"
#include "utilities.h"

#define C_PROMPT ": "
#define COMMENT_CHAR "#"
#define DEFAULT_DIR "HOME"


int exit_bltin(struct command* curr_command);
int cd_bltin(struct command* curr_command);
int status_bltin(struct command* curr_command);

int get_num_bltins(void);

int execute_command(struct command* curr_command);


char *bltin_funct_names[] = {
    "cd"
    //"exit",
    //"status"
};

int (*bltin_funct_ptrs[]) (struct command*) = {
    &cd_bltin
    //&exit_bltin,
    //&status_bltin
};

int get_num_bltins() {
    return sizeof(bltin_funct_names) / sizeof (char*);
}

int execute_command(struct command* curr_command) {
    
    int num_bltins = get_num_bltins();

    for (int index = 0; index < num_bltins; index++) {
        int comparison = strcmp(curr_command->args[index], bltin_funct_names[index]);
        if (comparison == 0) {
            return (*bltin_funct_ptrs[index]) (curr_command);
        }
    }
}



int cd_bltin(struct command* cd_command) {
    
    int chdir_return;
    
    if (cd_command->arg_count == 1) {
        char* default_dir = getenv("HOME");
        chdir_return = chdir(default_dir);
    } else {
        chdir_return = chdir(cd_command->args[1]);
    }

    char* new_dir = getcwd(NULL, 90);


    return chdir_return;
}

int main(void) {

    int run_flag = 1;
    pid_t shell_pid = getpid();
    char* shell_pid_str = int_to_dynstr(shell_pid);
    char expand_str[] = "$$";          //TO DO: change to a constanct char???

    while (run_flag) {
        printf(C_PROMPT);
        fflush(stdout);  
        struct command *curr_command = get_prelim_command();
        if (curr_command == NULL || is_comment(curr_command)) {
            continue;
        }
        expand_var(curr_command, expand_str, shell_pid_str);
        execute_command(curr_command);
        printf("%s\n", curr_command->args[0]);
     }

    return 0;
}
