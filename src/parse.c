/*
 *  swagger shell (swsh)
 *  Benjamin Roberts 2014
 *  COMP2300 Assignment One
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include "parse.h"
#include "jobs.h"
#include "debug.h"

/*
 * Define the various prompts the shell returns
 */
char SUCCESS_PROMPT[] = ">>> ";
char FAIL_PROMPT[] = "!!! ";
char ERROR_PROMPT[] = "??? ";

struct CommandEval * repl_read(int status)
{
    char * buffer;
    struct CommandEval * cmd;
    if(!(buffer = readline(get_prompt(status))))
        exit(0);

    if(DEBUG)
        printf("DEBUG (repl_read): %s\n", buffer);

    cmd = init_job(buffer);
    free(buffer);
    return cmd;
}

struct CommandEval * init_job(char input_buffer[])
{
    char * strtok_job;
    char * command_token = strtok_r(input_buffer, "|", &strtok_job);
    int cmd_pipe[2];

    struct CommandEval * job_head = init_command(command_token);
    struct CommandEval * job_last = job_head;
    job_head->pgid = calloc(1, sizeof(int));
    job_head->stdin = STDIN_FILENO;

    //Tokenize for piped commands
    while((command_token = strtok_r(0, "|", &strtok_job))) {
      job_last->next = init_command(command_token);

      //set up stdin/out pipes between last and next cmd
      if(pipe(cmd_pipe) >= 0) {
          job_last->stdout = cmd_pipe[1];
          job_last->next->stdin = cmd_pipe[0];
      }

      //set shared pgid and update last cmd
      job_last = job_last->next;
      job_last->pgid = job_head->pgid;
    }
    job_last->stdout = STDOUT_FILENO;
    return job_head;
}

struct CommandEval * init_command(char input_buffer[])
{
    char * strtok_cmd;
    struct CommandEval * cmd = calloc(1, sizeof(struct CommandEval));

    char * input_token = strtok_r(input_buffer, " ", &strtok_cmd);
    do {
        if (!cmd->name) { //first token will be cmd name
            cmd->vargs[0] = cmd->name = calloc(1, strlen(input_token)+1);
            strcpy(cmd->vargs[0], input_token);
            cmd->cargs++;
        } else if (!strcmp(input_token, "&")) { //check for BG task
            cmd->background = 1;
            break;
        } else if (strlen(input_token)) { //add argument to cmd
            cmd->vargs[cmd->cargs] = calloc(1, strlen(input_token)+1);
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
        printf("], BG: %d ", cmd->background);
        printf("STDIN: %d, STDOUT: %d\n", cmd->stdin, cmd->stdout);
        cmd = cmd->next;
    } while(cmd);
}

void free_command_eval(struct CommandEval * cmd)
{
    for(int i = 0; i < cmd->cargs; i++)
        free(cmd->vargs[i]);
    if(cmd->next)
      free_command_eval(cmd->next);
    else
      free(cmd->pgid); //Only free the pgid on the last cmd (as it is shared)
    free(cmd);
}

char * get_prompt(int status)
{
    if(status == 0)
        return SUCCESS_PROMPT;
    else if(status > 0)
        return FAIL_PROMPT;
    else
        return ERROR_PROMPT;
}


char * swsh_autocomplete_generator(const char * text, int state) {
    //cross call variables
    static char ** builtin_state;
    static int text_length;

    //initialise cross call variables if first call
    if(!state) {
        rl_set_prompt(ERROR_PROMPT);
        builtin_state = SWSH_BUILT_INS;
        text_length = strlen(text);
    }

    //check for built-in completion
    for(char ** builtin = builtin_state; *builtin; builtin++) {
        if(!strncmp(text, *builtin, text_length)) {
            char * ret_str = malloc(strlen(*builtin));
            strcpy(ret_str, *builtin);
            builtin_state = builtin + 1; //store current pos for next call
            return ret_str;
        }
    }

    return rl_filename_completion_function(text, state);
}



