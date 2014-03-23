#Current Bugs

 + foreground commands will hang on waitpid if background commands have been executed (OSX only)

 + small (const 5 byte) memory leak in readline rl_set_prompt().

 + pausing, resuming then quiting a job kills terminal input
