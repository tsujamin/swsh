/*  swagger shell (swsh) 
    Benjamin Roberts 2014
    COMP2300 Assignment One */

#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include "jobs.c"
        
int main(int argc, char * argv[])
{
    char input_buffer[80];
    
    while(scanf("%s", input_buffer)) {
        pid_t pid = vfork();
        if(pid == 0) {
            execlp(input_buffer, input_buffer, (char *) 0);
        } else if (pid < 0) {
            return -1;
        } 
    }
    return 0;
} 