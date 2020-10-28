#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "command.h"
#include "utilities.h"

#define C_PROMPT ":"


int main(void) {

    int run_flag = 1;
    pid_t shell_pid = getpid();
    char* shell_pid_str = int_to_dynstr(shell_pid);
    char expand_str[] = "$$";          //TO DO: change to a constanct char???

    while (run_flag) {
        printf(C_PROMPT);
        fflush(stdout);  
        struct command *curr_command = get_prelim_command();
        expand_var(curr_command, expand_str, shell_pid_str);
        printf("\n");
     }

    return 0;
}
