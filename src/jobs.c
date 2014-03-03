/*  
 *  swagger shell (swsh) 
 *  Benjamin Roberts 2014
 *  COMP2300 Assignment One 
 */
        
#include <sys/types.h>
#include <unistd.h>        
#include "jobs.h"
     
int vfork_eval(char input_buffer[])
{
    int child_exit = 0;
    pid_t pid = vfork();
    
    if(pid == 0) {
        execlp(input_buffer, input_buffer, (char *) 0);
    } else if (pid < 0) {
        return -1;
    } else {
        wait(&child_exit);
        return child_exit;
    }
}     
     
