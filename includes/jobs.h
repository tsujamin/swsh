/*  
 *  swagger shell (swsh) 
 *  Benjamin Roberts 2014
 *  COMP2300 Assignment One 
 */

#include "parse.h"

/*
 * executes the provided command (blocking the swsh instance).
 * returns -1 if fork fails, return code of child otherwise
 */
int vfork_eval(struct CommandEval cmd);     

