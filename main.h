/*
 *  swagger shell (swsh)
 *   Benjamin Roberts 2014
 *  COMP2300 Assignment One
 */

#ifndef MAIN_H
#define MAIN_H

#include<sys/types.h>

/*
 * Externally accessible properties of the root
 * shell process.
 */
extern int root_pgid, root_term;
extern struct termios termios_modes;
/*
 * Main call of the shell, contains the REPL loop
 */
int main(int argc, char * argv[]);

/*
 * Prepares the terminal environment
 */
void init_shell();

#endif /* MAIN_H */
