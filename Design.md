Design Notes: Unix Shell
1. Problem Analysis
Week 1:
Our first iteration was a static shell. It could only perform tasks we specifically wrote code for (cd and pwd). If a user wanted to run ls or mkdir, the shell was useless because it didn't know how to talk to the Operating System's program library. To create a real shell, the program must be able to take any command name, find that program on the disk, and run it as a separate process while the shell waits for it to finish.

2. Solution Architecture
Week 1:
Uses strtok to split the user's input string into an array of strings (args[]). This allows for flags like -l or -a to be passed correctly. Before trying to run a program, the shell checks if the command is built-in (like cd or exit). These must run in the parent process because they modify the shell's own state.
For all other commands, the shell uses the Fork-Exec-Wait pattern: 
1. Create a child process. 
2. Replace the child process with the desired program (e.g., ls).
3. The parent shell pauses until the child process is done.

3. Laboratory Implementation Timeline
Week 1:
Goal: Interactive Command Loop, Built-in Commands, External Command Execution
Week 2: 
Goal: I/O Redirection, Background Execution, Required System Calls
Week 3:
Goal: Data Structures, Process Execution, File Descriptor Management and Background Process Management
Week 4: 
Goal: Testing Strategy