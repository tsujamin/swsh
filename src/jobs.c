/*  
 *  swagger shell (swsh) 
 *  Benjamin Roberts 2014
 *  COMP2300 Assignment One 
 */
        
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>        
#include "jobs.h"
#include "parse.h"
     
int vfork_eval(struct CommandEval cmd)
{
    int child_exit = 0;
    pid_t pid = vfork();
    
    if(pid == 0) {
        execvp(cmd.name, cmd.vargs);
        exit(-1);
    } else if (pid < 0) {
        return -1;
    } else {
        wait(&child_exit);
        return child_exit;
    }
    return 0;
}     
     
