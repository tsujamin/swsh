/*
 *  swagger shell (swsh)
 *  Benjamin Roberts 2014
 *  COMP2300 Assignment One
 */

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <signal.h>
#include <readline/readline.h>
#include "main.h"
#include "debug.h"
#include "jobs.h"
#include "flags.h"
#include "parse.h"


int root_pgid, root_term;
struct termios termios_modes;

int main(int argc, char * argv[])
{
    init_shell();
    parse_swsh_flags(argc, argv);

    int eval_ret = 0;
    struct CommandEval * cmd;

    //Read-Print-Eval loop
    while(1) {
        cmd = repl_read(eval_ret);
        eval_ret = repl_eval(cmd);
        free_command_eval(cmd);
    }
    return 0;
}

void init_shell()
{
    //Ignore all signals
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    //Become our own process group
    root_pgid = getpid();
    setpgid(root_pgid, root_pgid);

    //Root store root terminal
    root_term = STDIN_FILENO;


    //Take control of the terminal
    tcsetpgrp(STDIN_FILENO, root_pgid);
    tcgetattr (STDIN_FILENO, &termios_modes);

    //set completion function for readline
    rl_completion_entry_function = &swsh_autocomplete_generator;

    //put hardcoded path in environ
    setenv("PATH", SWSH_PATH, 1);
    
}
