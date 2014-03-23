/*
 *  swagger shell (swsh)
 *  Benjamin Roberts 2014
 *  COMP2300 Assignment One
 */
#ifndef JOBS_H
#define JOBS_H
#include "parse.h"


/*
 * A null terminated array of command's supplied by (ie
 * not to be exec'd) the swsh
 */
extern char * SWSH_BUILT_INS[];

/*
 * Base function of the eval stage of the read-eval-print loop.
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

/*
 * Prints the pgids in the job stack. Defined as a built-in
 * in repl_eval
 */
void print_jobs();

/*
 * Responsible for setting the terminal controlling pgid, waiting
 * on the pgid, suspending it if stopped and restoring terminal control.
 * only nessecary if resuming a suspended job or upon reaching a
 * final foreground command
 */
void fg_wait_job(pid_t pgid);

/*
 * Pops a job from the job stack, continues and fg_wait_jobs it.
 * Prints error if empty job stack
 */
void resume_job();

/*
 * Adds a job to the job stack, prints error if full stack
 */
void suspend_job(pid_t pgid);


#endif /* JOBS_H */

