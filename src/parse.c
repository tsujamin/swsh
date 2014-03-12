/*
 *  swagger shell (swsh)
 *  Benjamin Roberts 2014
 *  COMP2300 Assignment One
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parse.h"
#include "debug.h"

#define INPUT_BUFFER_SIZE 80

struct CommandEval * repl_read()
{
    char input_buffer[INPUT_BUFFER_SIZE];

    if(!fgets(input_buffer, 80, stdin))
        exit(0);

    strtok(input_buffer, "\n"); //remove trailing newline

    if(DEBUG)
        printf("DEBUG (repl_read): %s\n", input_buffer);

    return init_job(input_buffer);
}

struct CommandEval * init_job(char input_buffer[])
{
    char *strtok_job;
    char * command_token = strtok_r(input_buffer, "|", &strtok_job);
    struct CommandEval * job_head = init_command(command_token);
    struct CommandEval * job_last = job_head;

    while((command_token = strtok_r(0, "|", &strtok_job))) {
      job_last->next = init_command(command_token);
      job_last = job_last->next;
    }

    return job_head;
}

struct CommandEval * init_command(char input_buffer[])
{
    char * strtok_cmd;
    struct CommandEval * cmd = malloc(sizeof(struct CommandEval));

    char * input_token = strtok_r(input_buffer, " ", &strtok_cmd);
    do {
        if (!cmd->name) { //first token will be cmd name
            cmd->vargs[0] = cmd->name = malloc(strlen(input_token)+1);
            strcpy(cmd->vargs[0], input_token);
            cmd->cargs++;
        } else if (!strcmp(input_token, "&")) {
            cmd->background = 1;
            break;
        } else if (strlen(input_token)) {
            cmd->vargs[cmd->cargs] = malloc(strlen(input_token)+1);
            strcpy(cmd->vargs[cmd->cargs], input_token);
            cmd->cargs++;
        }
    } while((input_token = strtok_r(0, " ", &strtok_cmd)));

    return cmd;
}


int delim_count(char str[], char delim)
{
    if (str == 0)
        return -1;

    int d_count = 0;
    for(int i = 0; str[i] != '\0'; i++) {
        if (str[i] == delim)
                d_count++;
    }
    return d_count;
}

void print_command_eval(struct CommandEval * cmd)
{
    do {
        printf("DEBUG (command_eval): ");
        printf("NAME: %s, ", cmd->name);
        printf("VARGS (%d): [ ", cmd->cargs);
        for(int i = 0; i < cmd->cargs; i++)
            printf("%s ", cmd->vargs[i]);
        printf("], BG: %d\n", cmd->background);

        cmd = cmd->next;
    } while(cmd);
}

void free_command_eval(struct CommandEval * cmd)
{
    for(int i = 0; i < cmd->cargs; i++)
        free(cmd->vargs[i]);
    if(cmd->next)
      free_command_eval(cmd->next);
    free(cmd);
}



