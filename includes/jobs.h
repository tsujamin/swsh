/*
 *  swagger shell (swsh)
 *  Benjamin Roberts 2014
 *  COMP2300 Assignment One
 */
#ifndef JOBS_H
#define JOBS_H
#include "parse.h"

/*
 * base function of the eval stage of the read-eval-print loop.
 * handles built-ins
 */
int repl_eval(struct CommandEval * cmd);

/*
 * vforks the shell process and calls proc_exec as the child
 */
int fork_eval(struct CommandEval * cmd);

/*
 * Sets the pgid of the child process, assigns it to
 * the shared job pgid, sets default signals and exec's the cmd
 */
void proc_exec(struct CommandEval * cmd);

#endif /* JOBS_H */

