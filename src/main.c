#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "command.h"
#include "utilities.h"

#define C_PROMPT ": "
#define COMMENT_CHAR "#"
#define DEFAULT_DIR "HOME"

#define NO_FG_RUN_YET "No foreground processes run yet, but per instructions, report exit status"
#define LAST_FG_TERMINATED "The last foreground process was terminated by signal"
#define LAST_FG_EXITED "The last foreground process terminated normally with exit status"



struct bgpid_node
{
    pid_t process_id;
    struct bgpid_node *next;
};

struct bgpid_node *bg_list_head = NULL;
struct bgpid_node *bg_list_tail = NULL;

int last_fg_endsig = 0;
char* last_fg_endmsg = NO_FG_RUN_YET;

//struct bgpid_node* create_bg_pidnode(pid_t process_id);

int exit_bltin(struct command* curr_command);
int cd_bltin(struct command* curr_command);
int status_bltin(struct command* curr_command);

int get_num_bltins(void);
int get_bltin_index(struct command* curr_command);

int execute_command(struct command* curr_command);
// int launch_foreground(struct command* curr_command);
//int launch_background(struct command* curr_command);




char *bltin_funct_names[] = {
    "cd",
    "status",
    "exit"
};

int (*bltin_funct_ptrs[]) (struct command*) = {
    &cd_bltin,
    &status_bltin,
    &exit_bltin
};

int get_num_bltins() {
    return sizeof(bltin_funct_names) / sizeof (char*);
}

int get_bltin_index(struct command* curr_command) {
    int bltin_index = -1;
    int num_bltins = get_num_bltins();
    for (int index = 0; index < num_bltins; index++) {
        int comparison = strcmp(curr_command->args[0], bltin_funct_names[index]);
        if (comparison == 0) {
            bltin_index = index;
        }
    }
    return bltin_index;
}

int execute_command(struct command* curr_command) {
    
    
    // Check if curr_command is a built-in function (and if it is, execute)
    //TO DO: remove intermediate num_bltins and comparison vars after debug done
    int num_bltins = get_num_bltins();
    for (int index = 0; index < num_bltins; index++) {
        int comparison = strcmp(curr_command->args[index], bltin_funct_names[index]);
        if (comparison == 0) {
            return (*bltin_funct_ptrs[index]) (curr_command);
        }
    }

    // if (curr_command->background) {
    //     return launch_background(curr_command);
    // } else {
    //     return launch_foreground(curr_command);
    // }  
}

void killall_bgprocs(struct bgpid_node* bgpid_list) {
    while (bgpid_list != NULL) {
        kill(bgpid_list->process_id, SIGKILL);
        bgpid_list = bgpid_list->next;
    }
}

int exit_bltin(struct command* exit_command) {

    killall_bgprocs(bg_list_head);

    return 0;

}

int status_bltin(struct command* status_command) {
    
    printf("%s %d\n", last_fg_endmsg, last_fg_endsig);

    return 1;

}

int cd_bltin(struct command* cd_command) {
    
    int chdir_return;
    
    if (cd_command->arg_count == 1) {
        char* default_dir = getenv("HOME");
        chdir_return = chdir(default_dir);
    } else {
        chdir_return = chdir(cd_command->args[1]);
    }

    // for testing only:
    //char* new_dir = getcwd(NULL, 90);

    return 1;
}

int main(void) {

    int run_flag = 1;
    pid_t shell_pid = getpid();
    char* shell_pid_str = int_to_dynstr(shell_pid);
    char expand_str[] = "$$";          //TO DO: Consider making this global and or a constant

    while (run_flag) {
        
        // TO DO: Need to check for and respond to completed bg processes
        
        printf(C_PROMPT);
        fflush(stdout);  
        struct command *curr_command = get_command(expand_str, shell_pid_str);
        
        // Check if for empty line or comment character
        if (curr_command == NULL || is_comment(curr_command)) {
            continue;
        }
              
        //Check if command is a "built-in" (and execut if it is)
        int bltin_index = get_bltin_index(curr_command);
        if (bltin_index >= 0) {
            run_flag = (*bltin_funct_ptrs[bltin_index]) (curr_command);
        }
     }
    return 0;
}
