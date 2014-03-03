/*  
 *  swagger shell (swsh) 
 *  Benjamin Roberts 2014
 *  COMP2300 Assignment One 
 */
        
#include <sys/types.h>
#include <unistd.h>        
#include "jobs.h"
#include "parse.h"
     
int vfork_eval(struct CommandEval cmd)
{
    int child_exit = 0;
    pid_t pid = vfork();
    
    if(pid == 0) {
        execlp(cmd.name, cmd.name, (char *) 0);
    } else if (pid < 0) {
        return -1;
    } else {
        wait(&child_exit);
        return child_exit;
    }
}     
     
