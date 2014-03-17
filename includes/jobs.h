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
 * Sets the childs pgid, closes the parent pipes and waits ones
 * final foreground task
 */
int fork_eval(struct CommandEval * cmd);

/*
 * Sets the pgid of the child process, assigns it to
 * the shared job pgid, sets default signals, duplicates relevant
 * file descriptors (closing useless ones to allow EOF to pipe)
 * and exec's the cmd
 */
void proc_exec(struct CommandEval * cmd);

#endif /* JOBS_H */

