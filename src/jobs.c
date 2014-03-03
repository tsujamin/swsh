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

int repl_eval(struct CommandEval cmd)
{
    if(!strcmp(cmd.name, "cd") && (cmd.cargs)) {
        chdir(cmd.vargs[0]);
        return 0;
    } else if (cmd.name){
        return vfork_eval(cmd);
    } else {
        exit(0);
    }
}     
    
     
int vfork_eval(struct CommandEval cmd)
{
    int child_exit = 0;
    pid_t pid = vfork();
    
    if(pid == 0) {
        execlp(cmd.name, cmd.name, *cmd.vargs, (char *) 0);
        exit(-1);
    } else if (pid < 0) {
        return -1;
    } else {
        wait(&child_exit);
        return child_exit;
    }
    return 0;
}     
     
