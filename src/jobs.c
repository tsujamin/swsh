/*
 *  swagger shell (swsh)
 *  Benjamin Roberts 2014
 *  COMP2300 Assignment One
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "jobs.h"
#include "parse.h"
#include "debug.h"


int repl_eval(struct CommandEval * cmd)
{
    int ret_status = 0;

    if (DEBUG)
        print_command_eval(cmd);

    for(; cmd; cmd = cmd->next) {
        if(!strcmp(cmd->name, "cd") && (cmd->cargs)) {
            chdir(cmd->vargs[1]);
        } else if (cmd->name) {
            ret_status = vfork_eval(cmd);
        } else {
            exit(0);
        }
    }

    return ret_status;
}



int vfork_eval(struct CommandEval * cmd)
{
    int child_exit = 0;
    pid_t pid = fork();

    if(pid == 0) {
        execvp(cmd->name, cmd->vargs);
        exit(-1);
    } else if (pid < 0) {
        return -1;
    } else {
        wait(&child_exit);
        return child_exit;
    }
    return 0;
}

