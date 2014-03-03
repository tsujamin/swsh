/*  
 *  swagger shell (swsh) 
 *  Benjamin Roberts 2014
 *  COMP2300 Assignment One 
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include "jobs.h"
        
int main(int argc, char * argv[])
{
    char input_buffer[80];
    int eval_ret = 0;
    struct CommandEval cmd;
    
    printf(">>> ");
    
    while(fgets(input_buffer, 80, stdin)) {
        cmd = init_command(input_buffer);
        eval_ret = vfork_eval(cmd);
        
        if (eval_ret < 0)
            printf("??? ");
        else if (eval_ret > 0)
            printf("!!! ");
        else
            printf(">>> ");
    }
    return 0;
} 