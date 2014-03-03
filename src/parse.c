/* 
 *  swagger shell (swsh) 
 *  Benjamin Roberts 2014
 *  COMP2300 Assignment One 
 */
        
#import <string.h>
#import "parse.h"
        
struct CommandEval init_command(char input_buffer[])
{        
    char * input_token;
    char delim[] = " ";
    struct CommandEval cmd = {0};
    printf("input_buffer: %s\n", input_buffer);
    
    while ((input_token = strtok(input_buffer, delim))) {
        if (!cmd.name) { //first token will be cmd name            
            cmd.name = input_token;
        } else if (!strcmp(input_token, "&")) {
            cmd.foreground = 1;
            break;
        } else {
            cmd.vargs[cmd.cargs] = input_token;
            cmd.cargs++;
        }
    }
    
    printf("cmd: %s\n", cmd.name);
    for(int i = 0; i < cmd.cargs; i++)
        printf("arg: %s\n", cmd.vargs[i]);
    printf("foreground: %d\n", cmd.foreground);
    
    return cmd;
}


int delim_count(char str[], char delim)
{
    if (str == 0)
        return -1;
    
    int d_count = 0;
    for(int i = 0; str[i] != '\0'; i++) {
        if (str[i] == delim)
                d_count++;
    }
    return d_count;    
}       
        