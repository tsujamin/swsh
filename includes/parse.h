/*  
 *  swagger shell (swsh) 
 *   Benjamin Roberts 2014
 *  COMP2300 Assignment One 
 */
        
#ifndef PARSE_H
#define PARSE_H        
struct CommandEval {
    char * name;
    char * vargs[10];
    int cargs;
    int foreground;
};

/*
 * parses the input and returns a  a struct with the name of
 * to eval, its parameters and foreground status (whether the
 * command was '&' appended)
 */      
struct CommandEval init_command(char input_buffer[]);
 
/*
 * counts the number of deliminating characters in the input string
 */      
int delim_count(char str[], char delim);
 
#endif /* PARSE_H */

