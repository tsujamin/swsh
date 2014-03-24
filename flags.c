/*
 *  swagger shell (swsh)
 *  Benjamin Roberts 2014
 *  COMP2300 Assignment One
 */

#include <string.h>
#include "flags.h"
#include "debug.h"

void parse_swsh_flags(int argc, char * argv[])
{
    for(int i = 1; i < argc; i++) {
        if(!strcmp(argv[i], "-d") || ~strcmp(argv[i], "--debug")) {
            DEBUG = 1;
        }
    }
}
