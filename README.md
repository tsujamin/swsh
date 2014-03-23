swagger shell (swsh)
====================

Benjamin Roberts 2014,
COMP2300 Assignment One

what is swsh?
-------------
swsh is a basic, low overhead, job controling command line shell. It was written as an assignment submission and as a general test of the authors C skills.

In the future I hope to keep it POSIX compliant and use it as a test bench for shell implementation experiments.


what can swsh do?
-----------------

A number of features you'd expect a command line shell to exhibit have been implemented in swsh including:

 + Execution of binaries by name from the /usr/bin and /bin folders

 + Execution of arbitrary binaries by relative or absolute location

 + Passing of command line arguments to commands

 + Ability to change working directory

 + stdin/stdout piping of commands

 + Stack based job control (suspension and resuming of jobs)
 
 + GNU/Bash readline based input (with custom completion)


using swsh
----------



license
-------
This software is licensed under the GNU General Public License v3. A copy of the license should have come bundled with the source code. In the event that a copy wasnt bundled please refer to [http://www.gnu.org/licenses/gpl-3.0.txt]()


the swsh internals
------------------



testing and reliability of swsh
-------------------------------



optional extensions of swsh
---------------------------
###Piping

###Job control

###Completion


limitations and bugs
--------------------

swsh currently has a handful of bugs. These can be seen in BUGS.md (found in the root of the project).

The shell also suffers from several limitations. These include:

 + A maximum of 10 arguments per command called

 + The shell no longer runs on OS X (the original development environment). The specific issue is noted in BUGS.md.

 + The job control is stack based. This means suspended jobs can only be resumed in a first-in-last-out fashion.
