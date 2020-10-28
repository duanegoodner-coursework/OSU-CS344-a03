#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "command.h"
#include "utilities.h"

#define C_PROMPT ":"
#define COMMENT_CHAR "#"

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
        printf("%s\n", curr_command->args[0]);
     }

    return 0;
}
