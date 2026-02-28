# CMSC125_Laboratory1: Unix Shell

# Complete Names of Group Members
Rainier RJ S. Espinal
Matthew F. Simpas

# Compilation and Usage Instructions
This project includes a `Makefile` for streamlined compilation.
To compile the shell, use the following commands:
1. make
2. make all
3. make clean

For manual compilation, use the following commands:
1. gcc -o mysh mysh.c parser.c external.c builtin.c jobs.c
2. ./mysh

Inside mysh, you can run built-in commands or any external program:
1. Built-ins: mysh > cd or mysh > pwd
2. External Programs: mysh > ls -la
3. I/O Redirection: mysh > sort < unsorted.txt > sorted.txt
4. Background Execution: mysh > sleep 10 &

# List of Implemented Features
1. Interactive Command Loop: Continuously prompts the user for input and non-blockingly reaps finished background processes.

2. Robust Command Parsing:
The parser tokenizes user input using strtok while supporting:
- Input/output redirection
- Background execution
- Argument parsing
- Buffer-safe command string construction for job tracking

3. Built-in Commands: Native execution of cd, pwd, status, and exit within the parent process.

4. External Command Execution: Utilizes the Fork-Exec-Wait pattern to spawn child processes for system commands.

![alt text](images/5c7a00d1-6305-4620-b053-4ecd2b26d2ed.jpg)

5. I/O Redirection: Supports input, output truncate, and output append with strict file descriptor management to prevent leaks.

![alt text](images/88af0ed5-cc26-41d6-bb57-d03b8787c63e.jpg)

6. Background Process Management: Tracks active background jobs, prevents zombie processes using waitpid() with WNOHANG, and notifies the user upon job completion

![alt text](images/14300815-5ba0-4bf4-90d7-95938c8e05b1.jpg)

# Known Limitations or Bugs
1. No Pipe Support: The shell does not currently support piping output between commands.

2. Quoted Arguments: The parser separates tokens strictly by whitespace. It does not support treating quoted strings with spaces as a single argument.

3. Hardcoded Limits: 
The shell maintains reasonable safety limits:
- Maximum of 255 command arguments
- Maximum of 64 background jobs tracked simultaneously

These limits help maintain predictable memory usage while supporting common command workloads.

4. Redirection Syntax: Redirection tokens must be separated by spaces.

# Design Decisions and Architecture Overview
The shell is built using a highly modular architecture emphasizing the separation of concerns, memory safety, and strict system call error handling.
1. Decoupled Parsing and Execution: Instead of raw string array manipulation during the execution phase, the shell uses structure.h. The parser.c module is solely responsible for parsing tokens and populating this struct. The external.c module blindly executes the instructions within the struct, adhering strictly to the Single Responsibility Principle.

2. Memory Management: 
The shell balances safety and flexibility by combining:
- Stack-based structures for command state
- Dynamic memory allocation for input and token storage

All allocated memory is explicitly freed after command execution.

3. File Descriptor Management: To prevent file descriptor leaks, dup2 is immediately followed by close() for any file opened during I/O redirection, guaranteeing the child process executes in a clean environment.

4. Job Control Module: Background process tracking is extracted into a dedicated jobs.c module. It maintains a stateful array of active PIDs and original command strings. At the start of every REPL iteration, the shell queries this list to clean up terminated child processes and print status updates, completely eliminating zombie processes