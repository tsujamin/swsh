/*
 *  swagger shell (swsh)
 *  Benjamin Roberts 2014
 *  COMP2300 Assignment One
 */
#ifndef FLAGS_H
#define FLAGS_H

#include "debug.h"

/*
 * Evaluates the arguments provided to swsh at startup
 * FLAGS:
 *   -d: debug mode
 */
void parse_swsh_flags(int cargs, char * vargs[]);

#endif /* FLAGS_H */
