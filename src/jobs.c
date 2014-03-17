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

    //iterates through commands and exec's or parses built-in
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
    } else { //parent branch
        //set child pgid (apparently can cause race condition without)
        if (*cmd->pgid == 0)
            *cmd->pgid = pid;
        setpgid(pid, *cmd->pgid);

        //close unused parent pipes (allows EOF to pipe)
        if (cmd->stdin != STDIN_FILENO)
          close(cmd->stdin);
        if (cmd->stdout != STDOUT_FILENO)
          close(cmd->stdout);

        //only wait/grant control if non-background final cmd
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
    //set pgid (first cmd's pid is pgid)
    pid_t pid = getpid();
    if (*cmd->pgid == 0)
        *cmd->pgid = pid;
    setpgid(pid, *cmd->pgid);

    //undo job-control signal ignores
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    signal(SIGCHLD, SIG_DFL);

    //dup file descriptios for stdin/out
    if(cmd->stdin != STDIN_FILENO) {
        dup2(cmd->stdin, STDIN_FILENO);
        close(cmd->stdin);
    }

    if(cmd->stdout != STDOUT_FILENO) {
        dup2(cmd->stdout, STDOUT_FILENO);
        close(cmd->stdout);
    }

    //takes terminal control if final and non-background
    if(!cmd->next && !cmd->background)
        tcsetpgrp(root_term, *cmd->pgid);

    execvp(cmd->name, cmd->vargs);
}




