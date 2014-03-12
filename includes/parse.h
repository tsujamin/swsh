/*
 *  swagger shell (swsh)
 *   Benjamin Roberts 2014
 *  COMP2300 Assignment One
 */

#ifndef PARSE_H
#define PARSE_H
struct CommandEval {
    char *  name;
    char *  vargs[10];
    int     cargs;
    int     background;
    int  *  pgid;
    struct CommandEval * next;

};

/*
 * entry point for the read stage of the repl loop.
 * returns a CommandEval stuct.
 */
struct CommandEval * repl_read();

/*
 * parses command line input tokenising it on '|' and
 * parsing it with init_command before returning
 * the head of a CommandEval list
 */
struct CommandEval * init_job(char input_bugger[]);


/*
 * parses the command string and returns a struct with the name of
 * to eval, its parameters and foreground status (whether the
 * command was '&' appended)
 */
struct CommandEval * init_command(char input_buffer[]);

/*
 * counts the number of deliminating characters in the input string
 */
int delim_count(char str[], char delim);

/*
 * print a debug representation of a CommandEval struct
 */
void print_command_eval(struct CommandEval *cmd);

/*
 * deallocates calloced memory in CommandEval and its linked
 * counterparts
 */
void free_command_eval(struct CommandEval * cmd);

#endif /* PARSE_H */


