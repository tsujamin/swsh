/*
 *  swagger shell (swsh)
 *  Benjamin Roberts 2014
 *  COMP2300 Assignment One
 */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "debug.h"
#include "jobs.h"
#include "flags.h"

int main(int argc, char * argv[])
{
    parse_swsh_flags(argc, argv);

    int eval_ret = 0;
    struct CommandEval cmd;

    printf(">>> ");

    while(1) {
        cmd = repl_read();
        eval_ret = repl_eval(cmd);
        free_command_eval(cmd);

        if (eval_ret < 0)
            printf("??? ");
        else if (eval_ret > 0)
            printf("!!! ");
        else
            printf(">>> ");
    }
    return 0;
}
