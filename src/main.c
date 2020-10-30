#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include "command.h"
#include "utilities.h"

#define C_PROMPT ": "
#define COMMENT_CHAR "#"
#define DEFAULT_DIR "HOME"

#define NO_FG_RUN_YET "No foreground processes run yet, but per instructions, report exit status"
#define LAST_FG_TERMINATED "The last foreground process was terminated by signal"
#define LAST_FG_EXITED "The last foreground process terminated normally with exit status"

// TO DO: add fflush() after every print statement

struct bgpid_node
{
    pid_t process_id;
    struct bgpid_node *next;
};
struct bgpid_node *bg_list_head = NULL;
struct bgpid_node *bg_list_tail = NULL;
//struct bgpid_node* create_bg_pidnode(pid_t process_id);

int last_fg_endsig = 0;
char* last_fg_endmsg = NO_FG_RUN_YET;
bool last_fg_terminated = false;

bool bg_launch_allowed = true;
bool potential_zombies = false;

int cd_bltin(struct command* cd_command);
int status_bltin(struct command* status_command);
int exit_bltin(struct command* exit_command);

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

void killall_bgprocs(struct bgpid_node* bgpid_list) {
    while (bgpid_list != NULL) {
        kill(bgpid_list->process_id, SIGKILL);
        bgpid_list = bgpid_list->next;
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

    // for testing only:
    //char* new_dir = getcwd(NULL, 90);

    return 1;
}

int status_bltin(struct command* status_command) {
    
    printf("%s %d\n", last_fg_endmsg, last_fg_endsig);

    return 1;

}

int exit_bltin(struct command* exit_command) {

    killall_bgprocs(bg_list_head);

    return 0;
}


#define ENTER_FG_ONLY_MSG "Entering foreground-only mode (& is now ignored)\n"
#define EXIT_FG_ONLY_MSG "Exiting foreground-only mode\n"
void handle_SIGTSTP (int signo) {
    //TO DO: modify so handler only changes bool, then use separate function
    //to output messages at start of main while loop.
    bg_launch_allowed = !bg_launch_allowed;
    if (!bg_launch_allowed) {
        write(STDOUT_FILENO, ENTER_FG_ONLY_MSG, 49);
    } else {
        write(STDOUT_FILENO, EXIT_FG_ONLY_MSG, 29);
    }
    write(STDOUT_FILENO, C_PROMPT, 2);       
}

void handle_SIGCHLD (int signo) {
    potential_zombies = true;
}

void set_shell_sighandlers() {
    struct sigaction ignore_action = {0};
    ignore_action.sa_handler = SIG_IGN;
    sigaction(SIGINT, &ignore_action, NULL);

    struct sigaction SIGTSTP_action = {0};
    SIGTSTP_action.sa_handler = handle_SIGTSTP;
    SIGTSTP_action.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &SIGTSTP_action, NULL);

    struct sigaction SIGCHLD_action = {0};
    SIGCHLD_action.sa_handler = handle_SIGCHLD;
    SIGCHLD_action.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &SIGCHLD_action, NULL);
}

void set_fgchild_sighandlers() {
    struct sigaction SIGINT_action = {0};
    SIGINT_action.sa_handler = SIG_DFL;
    SIGINT_action.sa_flags = SA_RESTART; // Is SA_RESTART flag necessary???
    sigaction(SIGINT, &SIGINT_action, NULL);
}

int redirect_ouptut(char* new_out_path) {
    int out_fd = open(new_out_path, O_WRONLY | O_CREAT | O_TRUNC, 0640);
    if (out_fd == -1) {
        fprintf(stderr, "Failed open %s for output", new_out_path);
        exit(1);
    }

    int dup2_result = dup2(out_fd, 1);
    if (dup2_result == -1) {
        perror("output dup2 error");
        exit(2);
    }
}

void redirect_input(char* new_in_path) {
    int in_fd = open(new_in_path, O_RDONLY);
    if (in_fd == -1) {
        fprintf(stderr, "cannot open %s for input", new_in_path);
        exit(1);
    }

    int dup2_result = dup2(in_fd, 0);
    if (dup2_result == -1) {
        fprintf(stderr, "input dup2 error");
    }
}

int launch_foreground(struct command* curr_command) {

    int fgchild_status;
    pid_t fgchild_pid = fork();

    if (fgchild_pid == -1) {
        perror("fork() failed.");
        exit(1);
    
    // child branch
    } else if (fgchild_pid == 0) {
        
        // add signal handlers
        set_fgchild_sighandlers();
        
        // deal with redirects
        if (curr_command->output_redirect != NULL) {
            redirect_ouptut(curr_command->output_redirect);
        }
        if (curr_command->input_redirect != NULL) {
        redirect_input(curr_command->input_redirect);
        }
              
        // use execv to load and run new program
        execvp(curr_command->args[0], curr_command->args);

        // if execv fails:
        fprintf(stderr, "could not find command %s", curr_command->args[0]);
        exit(1);

    // parent branch
    } else {
        fgchild_pid = waitpid(fgchild_pid, &fgchild_status, 0);
        if (WIFEXITED(fgchild_status)) {
            last_fg_endmsg = LAST_FG_EXITED;
            last_fg_endsig = WEXITSTATUS(fgchild_status);
        } else {
            last_fg_endmsg = LAST_FG_TERMINATED;
            last_fg_endsig = WTERMSIG(fgchild_status);
            last_fg_terminated = true;
        }
    }

    return 1; // need this val because run_flag = 1 causes main while loop to repeat
    // TO DO: consider making run_flag = 0 cause while loop to continue
    // so that everything except for built-in exit can return 0 (more conventional)
}

void force_report_last_fg_end(void) {
    printf("%s %d\n", last_fg_endmsg, last_fg_endsig);
    last_fg_terminated = false; 
}

int launch_background(struct command* curr_command);


int main(void) {

    int run_flag = 1;
    pid_t shell_pid = getpid();
    char* shell_pid_str = int_to_dynstr(shell_pid);
    char expand_str[] = "$$";          //TO DO: Consider making this global and or a constant

    set_shell_sighandlers();
    
    while (run_flag) {
        
        // TODO: Check for and respond to completed bg processes
        // TODO: Modify SIGSTP handler so that it only changes bool, then put msg function here
        // TODO: Consider placing pointers to any pre-prompt messages in an array?? 
        if (last_fg_terminated) {
            force_report_last_fg_end();
        }
        printf(C_PROMPT);
        fflush(stdout);  
        struct command *curr_command = get_command(expand_str, shell_pid_str);
        
        // Check if for empty line or comment character
        if (curr_command == NULL || is_comment(curr_command)) {
            continue;
        }
              
        //Check if command is a "built-in" (and execute if it is)
        int bltin_index = get_bltin_index(curr_command);
        if (bltin_index >= 0) {
            run_flag = (*bltin_funct_ptrs[bltin_index]) (curr_command);
        } 
        // else if (bg_launch_allowed && curr_command->background) {
        //     run_flag = launch_background(curr_command);
        // }
        else {
            run_flag = launch_foreground(curr_command);
        }
     }

    return 0;
}
