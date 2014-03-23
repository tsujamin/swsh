swagger shell (swsh)
====================

Benjamin Roberts 2014,
COMP2300 Assignment One

what is swsh?
-------------
swsh is a basic, low overhead, job controling command line shell. It was written as an assignment submission and as a general test of the authors C foo.

In the future I hope to keep it POSIX compliant and use it as a test bench for shell implementation experiments.


what can swsh do?
-----------------

A number of features you'd expect a command line shell to exhibit have been implemented in swsh including:

 + Execution of binaries by name from the /usr/bin and /bin folders

 + Execution of arbitrary binaries by relative or absolute location

 + Passing of command line arguments to commands

 + Ability to change working directory

 + stdin/stdout piping of commands

 + Stack based job control (with stack flippig and suspension/resumption of jobs)

 + GNU/Bash readline based input (with custom written path based completion)


using swsh
----------
swsh works in a similar manner to most other shell programs. Commands are entered, evaluated and returned.

If a user enters a command, eg `echo Hello World`, the shell searches the `/bin`,`/usr/bin` and the users current directory for the command `echo`. It subsequently executes it with the two arguments `Hello World`.

swsh allows a user to chain together commands by piping the output of one to the input of another. An example of such piping would be the expression `ls -la | grep ".mp3" | wc -l` which would count the number of mp3's in the current directory.

swsh is a job controlling shell. It allows the user to spawn jobs in the background by appending the ampersand symbol to a command (eg `nano &`) or suspend current jobs with the Ctrl+Z command. Jobs are placed on a job stack from which they can be popped off into the foreground again.

This job stack can be inverted giving the user the ability to pop jobs off the opposite end of the stack. At most 10 jobs can be suspended onto the job stack at any one time.

If a user presses the tab key they will see their prompt change to `???`. This means they are entering competion mode. A futher pressing of the tab key will list the possible commands/files the user could have been refering to. If only one such entry exists it is automatically entered for the user.

swsh has several built in commands including:

 + `cd <dir>`: Changes the working directory of the shell to that specified by `<dir>`

 + `jobs`: Lists the jobs currently suspended on the job stack

 + `resume`: Pops a job off the top of the stack and resumes it in the foreground

 + `flip`: Flips the order of jobs on the jobstack

 + `quit`: Exits the terminal (aswell as `Ctrl+D`)


license
-------
This software is licensed under the GNU General Public License v3. A copy of the license should have come bundled with the source code. In the event that a copy wasnt bundled please refer to [http://www.gnu.org/licenses/gpl-3.0.txt]()


the swsh internals
------------------
Internally, swsh is simply a read-eval loop repeatedily calling the functions `repl_read(int status)` and `repl_eval(struct CommandEval * cmd)`.

`repl_read` is responsible for the input prompt (a GNU readline input with completion) and tokenising/parsing the input buffer to form chains of `struct CommandEval`'s. This is the primary struct the shell is based around. It stores the command name and arguents to be executed, the input and output file descriptors and exists as a node in a linked list of other `struct CommandEval`'s for simple parsing.

As mentioned previously the shell uses GNU readline as its input prompt but with a custom completion generator function `swsh_auto_complete_generator(const char * text, int status)`. This is a re-enteratnt  method called by readline when the user prompts for completion. It iterates though the list of built-in swsh commands, aswell as all the files reachable in the `PATH`variable, and returns those which are identical to the input text segment (up until the text segment ends).

The `repl_eval(struct CommandEval * cmd)` function initiates the execution of a linked list of `struct CommandEval`'s (refered to as a job). It internally handles those commands which resolve to swsh built-ins and fork/exec's the remaining commands; placing them in the same process group and connecting input/output pipes as nessecary.

If the job was flagged as a background job (with the ampersand symbol) then it is suspended and control returns to `repl_read(int status)`, otherwise the shell waits on the completion or suspension of the job. Suspended jobs are placed on the job stack and can be viewed and resumed with the appropriate built-in command.


testing and reliability of swsh
-------------------------------
swsh has been manually tested and found to be robust. Each feature was tested and corner case bugs were eliminated or documented in `BUGS.md`. The shell was developed under revision control so that the causes of new bugs and regressions could be quickly identified.

As manual allocation and freeing of memory was used widly in the shell, great consideration was payed to preventing memory leaks. After major feature updates the shell was tested under `valgrind --tool=memcheck` for any unanticipated leaks. These were patched and at the time of writing there were no leaks in the swsh source code (however one does register from within `libreadline`).


optional extensions of swsh
---------------------------
###Piping
swsh includes an input/output piping mechanism for commands. Commands seperated by the pipe character (eg. `echo no | yes`) are placed sequentially in a linked list of `struc CommandEval`'s and have their stdin/stdout fields set to the appropriate ends of a newly created unix `pipe()`. This action occures in `init_job(char input_buffer[])`.

The child process forked as part of the execution of a command will duplicate the provided pipe file descriptors in place of its stdin/stdout close the unused file descriptors that remain. The result of this is that the output of one command will become the input of the next: enhancing the overall usefuleness of the shell.


###Job control
The shell provides a stackbased job-control implementation. The implmentation of this system involves the suspension and resumption of whole process groups and the storage of their associated id. All commands executed as part of a job are placed in the same process group and have their signal behaviours set to default (they initially inherit the ignore all behaviour of the parent shell).

Using the `<termios.h>` library the foreground job is granted control of the terminal session. This control remains untill it is suspended or exits naturally; as of which control is regained by the shell's process group. Due to the ability of running jobs to alter the configuration of the terminal, the default settings are always restored when the shell regains foreground control.

Suspended jobs have their process group id's and names stored on a stack and can be popped off, viewed or have their orders reversed use of the built-in commands.

###Completion
The shell implements its own completion generator which it supplies to readline. This was done for two reasons:
 + The use of readline removed the need for me to implement a great deal of input logic (such as in-line editing). I don't see the need to re-invent the wheel when the community has already provided a solution, however I didn't want to be entirely reliant upon it either.

 + The default completion generator bundled with readline was missing `PATH` completion and would  not return the built-in commands of swsh. It was still initially going to be used for its filename completion but it turned out simpler to implement that aswell.

The completion function is re-enterant and as such stores much of its state in static varibles. Upon re-entry it can resume searching through both the built-in commands and directory descriptors at the position it previously left off. It uses `readdir_r` to iterate though the entries of each `PATH` directory's directory stream whilst scanning for matches with `strcmpn`.

limitations and bugs
--------------------

swsh currently has a handful of bugs. These can be seen in `BUGS.md` (found in the root of the project).

The shell also suffers from several limitations. These include:

 + A maximum of 10 command line arguments per line is supported. This is simply a macro defined limit and could be increased or removed if a variable size arguments array was implemented. A similar restricion with similar fixes is the 10 job limit of the job stack.

 + No `bash` like string support. This is not entirely an issue as swsh has very few reserved characters (only `|` and `&`) but it would be a desirable feature.

 + No history support. This could be enabled with `<readline/history.h>` but I refrained as a `zsh` style history system would be preferable (and I may implement it in the future).

 + The shell isn't completly functional on Mac OS 10.9 (its original development system) as suspending jobs causes a hang. This is because of a difference in the `fork()/execvp()/waitpid()` of Linux (where the shell is functional) and OS X. This is somewhat ironic given that OS X is a certified POSIX system unlike Linux. The specific issue is noted in `BUGS.md`.

 + The job control is stack based (ie. suspended jobs are LIFO) which may appear strange to users of `bash`and other *nix based shells. This was infact a design choice as I was intersted in alternate models of job control in command line shells.
