/*
 *  swagger shell (swsh)
 *  Benjamin Roberts 2014
 *  COMP2300 Assignment One
 */
#include "main.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "jobs.h"
#include "parse.h"
#include "debug.h"

/*
 * An array to store the job stack in. Any process group which
 * is suspended is added to this array (if doing so doesnt overflow.
 */
#define MAX_SUSP_JOBS 10
pid_t * susp_jobs_pgid_stack = 0;
char ** susp_jobs_names = 0;
int   susp_jobs_count = 0;

char * SWSH_BUILT_INS[] = {"cd", "resume", "jobs", "flip","quit", 0};

char SWSH_PATH[] = "/bin:/usr/bin:.";

char * LINUX_SWSH_PATH[] = {"/bin", "/usr/bin", ".", 0};

int repl_eval(struct CommandEval * cmd)
{
    int ret_status = 0;

    if (DEBUG)
        print_command_eval(cmd);

    //initialise jobs stack
    if (!susp_jobs_pgid_stack) {
        susp_jobs_pgid_stack = calloc(MAX_SUSP_JOBS, sizeof(pid_t));
        susp_jobs_names = calloc(MAX_SUSP_JOBS, sizeof(char *));
    }
    //iterates through commands and exec's or parses built-in
    for(; cmd; cmd = cmd->next) {
        if(!strcmp(cmd->name, "cd") && (cmd->cargs)) {
            chdir(cmd->vargs[1]);
        } else if (!strcmp(cmd->name, "resume")) {
            resume_job();
        } else if (!strcmp(cmd->name, "jobs")) {
            print_jobs();
        } else if (!strcmp(cmd->name, "flip")) {
            flip_jobs();
        } else if (!strcmp(cmd->name, "quit")) {
            exit(0);
        } else if (cmd->name) {
            ret_status = fork_eval(cmd);
        } else {
            exit(0);
        }
    }

    return ret_status;
}



int fork_eval(struct CommandEval * cmd)
{
    pid_t pid = fork();

    if(pid == 0) {
        proc_exec(cmd);
        exit(-1);
    } else if (pid < 0) {
        return -1;
    } else { //parent branch
        //set child pgid (apparently can cause race condition without)
        if (*cmd->pgid == 0)
            *cmd->pgid = pid;
        setpgid(pid, *cmd->pgid);

        //close unused parent pipes (allows EOF to pipe)
        if (cmd->stdin != STDIN_FILENO)
            close(cmd->stdin);
        if (cmd->stdout != STDOUT_FILENO)
            close(cmd->stdout);

        //wait or suspend final cmd if nessecary
        if(!cmd->next && !cmd->background)
            fg_wait_job(*cmd->pgid, cmd->name);
        else if(!cmd->next)
            suspend_job(*cmd->pgid, cmd->name);

        return 0;
    }
    return 0;
}

void proc_exec(struct CommandEval * cmd)
{
    //set pgid (first cmd's pid is pgid)
    pid_t pid = getpid();
    if (*cmd->pgid == 0)
        *cmd->pgid = pid;
    setpgid(pid, *cmd->pgid);

    //undo job-control signal ignores
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    signal(SIGCHLD, SIG_DFL);

    //dup file descriptios for stdin/out
    if(cmd->stdin != STDIN_FILENO) {
        dup2(cmd->stdin, STDIN_FILENO);
        close(cmd->stdin);
    }

    if(cmd->stdout != STDOUT_FILENO) {
        dup2(cmd->stdout, STDOUT_FILENO);
        close(cmd->stdout);
    }

    //takes terminal control if final and non-background
    if(!cmd->next && !cmd->background)
        tcsetpgrp(root_term, *cmd->pgid);

    #ifdef __APPLE__
        execvP(cmd->name, SWSH_PATH, cmd->vargs);
    #elif __linux__
        execvpe(cmd->name, cmd->vargs, LINUX_SWSH_PATH);
    #endif
}

void print_jobs()
{
    if(susp_jobs_count > 0) {
        //iterate through job list in descending order
        for(int i = susp_jobs_count - 1; i >= 0; i--)
            printf("%s %d\n", susp_jobs_names[i], susp_jobs_pgid_stack[i]);
    } else {
        printf("No jobs in stack\n");
    }
}


void suspend_job(pid_t pgid, char * name)
{
    //add job to susp_jobs_pgid_stack list before inc'ing count
    if(susp_jobs_count < MAX_SUSP_JOBS) {
        susp_jobs_pgid_stack[susp_jobs_count] = pgid;
        susp_jobs_names[susp_jobs_count] = malloc(strlen(name)+1);
        strcpy(susp_jobs_names[susp_jobs_count], name);
        susp_jobs_count++;
    } else {
        printf("Job stack full\n");
    }
}

void resume_job()
{
    if(susp_jobs_count > 0) {
        susp_jobs_count--; //pop job

        //continue job and foreground wait it
        kill (susp_jobs_pgid_stack[susp_jobs_count], SIGCONT);

        //if wait returns 0 (exited) free the job name
        if(!fg_wait_job(susp_jobs_pgid_stack[susp_jobs_count],
                        susp_jobs_names[susp_jobs_count])) {
            free(susp_jobs_names[susp_jobs_count]);
        }
    } else {
        printf("No jobs in stack\n");
    }
}

int fg_wait_job(pid_t pgid, char * name)
{
    int child_status = 0;

    //give job the terminal
    tcsetpgrp(root_term, pgid);

    //wait and suspend if stopped
    waitpid(-pgid, &child_status, WUNTRACED);
    if(WIFSTOPPED(child_status)) {
        suspend_job(pgid, name);
        return 1;
    }
    //take back terminal
    tcsetpgrp(root_term, root_pgid);

    return 0;
}

void flip_jobs()
{
    pid_t * new_jobs_stack = calloc(MAX_SUSP_JOBS, sizeof(pid_t));
    char ** new_names = calloc(MAX_SUSP_JOBS, sizeof(char *));

    //flip the job stack into new array
    for (int i = 1; i <= susp_jobs_count; i++) {
        new_jobs_stack[i-1] = susp_jobs_pgid_stack[susp_jobs_count - i];
        new_names[i-1] = susp_jobs_names[susp_jobs_count - i];
    }
    //replace old stack
    free(susp_jobs_pgid_stack);
    free(susp_jobs_names);
    susp_jobs_pgid_stack = new_jobs_stack;
    susp_jobs_names = new_names;
    printf("Flipped jobs stack\n");
}




