/*  
 *  swagger shell (swsh) 
 *  Benjamin Roberts 2014
 *  COMP2300 Assignment One 
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include "jobs.c"
        
int main(int argc, char * argv[])
{
    char term_prompt[] = ">>> ";
    char input_buffer[80];
    
    printf("%s", term_prompt);
    
    while(scanf("%s", input_buffer) > 0) {
        pid_t pid = vfork();
        if(pid == 0) {
            execlp(input_buffer, input_buffer, (char *) 0);
        } else if (pid < 0) {
            printf("!!! ");
        } else {
            int child_exit;
            wait(&child_exit);
            
            if(child_exit) 
                printf("!!! ");
            else 
                printf("%s", term_prompt);
        }
    }
    return 0;
} 