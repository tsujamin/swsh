/*
 *  swagger shell (swsh)
 *  Benjamin Roberts 2014
 *  COMP2300 Assignment One
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "jobs.h"
#include "parse.h"
#include "debug.h"
#include "main.h"


int repl_eval(struct CommandEval * cmd)
{
    int ret_status = 0;

    if (DEBUG)
        print_command_eval(cmd);

    for(; cmd; cmd = cmd->next) {
        if(!strcmp(cmd->name, "cd") && (cmd->cargs)) {
            chdir(cmd->vargs[1]);
        } else if (cmd->name) {
            ret_status = fork_eval(cmd);
        } else {
            exit(0);
        }
    }

    return ret_status;
}



int fork_eval(struct CommandEval * cmd)
{
    int child_exit = 0;
    pid_t pid = fork();

    if(pid == 0) {
        proc_exec(cmd);
        exit(-1);
    } else if (pid < 0) {
        return -1;
    } else {
        if (*cmd->pgid == 0)
            *cmd->pgid = pid;
        setpgid(pid, *cmd->pgid);

        if(!cmd->next && !cmd->background) {
            tcsetpgrp(root_term, *cmd->pgid);
            waitpid(-*cmd->pgid, &child_exit, WUNTRACED);
            tcsetpgrp(root_term, root_pgid);
        }
        return child_exit;
    }
    return 0;
}

void proc_exec(struct CommandEval * cmd)
{
    pid_t pid = getpid();
    if (*cmd->pgid == 0)
        *cmd->pgid = pid;
    setpgid(pid, *cmd->pgid);

    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    signal(SIGCHLD, SIG_DFL);

    if(!cmd->next && !cmd->background)
        tcsetpgrp(root_term, *cmd->pgid);

    execvp(cmd->name, cmd->vargs);
}




