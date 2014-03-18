swagger shell (swsh)
====================

Benjamin Roberts 2014,
COMP2300 Assignment One

what is swsh?
-------------



what can swsh do?
-----------------

A number of features you'd expect a command line shell to exhibit have been implemented in swsh including:

 + Execution of binaries by name from the /usr/bin and /bin folders

 + Execution of arbitrary binaries by relative or absolute location

 + Passing of command line arguments to commands

 + Ability to change working directory

 + stdin/stdout piping of commands

 + Stack based job control (suspension and resuming of jobs)


using swsh
----------



license
-------



the swsh internals
------------------



testing and reliability of swsh
-------------------------------



optional extensions of swsh
---------------------------
###Piping

###Job control



limitations and bugs
--------------------

swsh currently has a handful of bugs. These can be seen in BUGS.md (found in the root of the project).

The shell also suffers from several limitations. These include:

 + A maximum of 10 arguments per command called

 + The shell no longer runs on OS X (the original development environment). The specific issue is noted in BUGS.md.

 + The job control is stack based. This means suspended jobs can only be resumed in a first-in-last-out fashion.
