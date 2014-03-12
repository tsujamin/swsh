/*
 *  swagger shell (swsh)
 *  Benjamin Roberts 2014
 *  COMP2300 Assignment One
 */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <signal.h>
#include "main.h"
#include "debug.h"
#include "jobs.h"
#include "flags.h"

int root_pgid;
struct termios termios_modes;

int main(int argc, char * argv[])
{
    init_shell();
    parse_swsh_flags(argc, argv);

    int eval_ret = 0;
    struct CommandEval cmd;

    printf(">>> ");

    while(1) {
        cmd = repl_read();
        eval_ret = repl_eval(cmd);
        free_command_eval(cmd);

        if (eval_ret < 0)
            printf("??? ");
        else if (eval_ret > 0)
            printf("!!! ");
        else
            printf(">>> ");
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

    //Take control of the terminal
    tcsetpgrp(STDIN_FILENO, root_pgid);
    tcgetattr (STDIN_FILENO, &termios_modes);
}
