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
int repl_eval(struct CommandEval cmd);

/*
 * executes the provided command (blocking the swsh instance).
 * returns -1 if fork fails, return code of child otherwise
 */
int vfork_eval(struct CommandEval cmd);

#endif /* JOBS_H */

