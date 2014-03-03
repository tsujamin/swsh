/* 
 *  swagger shell (swsh) 
 *  Benjamin Roberts 2014
 *  COMP2300 Assignment One 
 */
        
#import <string.h>
#import "parse.h"
        
struct CommandEval init_command(char input_buffer[])
{        
    char delim[] = " ";
    struct CommandEval cmd = {0};
    
    char * input_token = strtok(input_buffer, delim);
    do {
        if (!cmd.name) { //first token will be cmd name            
            cmd.name = input_token;
        } else if (!strcmp(input_token, "&")) {
            cmd.foreground = 1;
            break;
        } else {
            cmd.vargs[cmd.cargs] = input_token;
            cmd.cargs++;
        }
    } while((input_token = strtok(0, delim)));
    
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
        