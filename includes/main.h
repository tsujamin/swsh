/*
 *  swagger shell (swsh)
 *   Benjamin Roberts 2014
 *  COMP2300 Assignment One
 */
/*
 * Linux port requres gnu source for execvpe
 */
#define _GNU_SOURCE

#ifndef MAIN_H
#define MAIN_H

/*
 * Externally accessible properties of the root
 * shell process.
 */
extern int root_pgid, root_term;

/*
 * Main call of the shell, contains the REPL loop
 */
int main(int argc, char * argv[]);

/*
 * Prepares the terminal environment
 */
void init_shell();

#endif /* MAIN_H */
