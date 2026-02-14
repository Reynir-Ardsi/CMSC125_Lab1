# CMSC125_Laboratory1: Unix Shell

# Complete Names of Group Members
Rainier RJ E. Espinal
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
2. Robust Command Parsing: Safely tokenizes input, handling edge cases like multiple spaces, trailing whitespaces, and buffer overflow protection.
3. Built-in Commands: Native execution of cd, pwd, status, and exit within the parent process.
4. External Command Execution: Utilizes the Fork-Exec-Wait pattern to spawn child processes for system commands.
5. I/O Redirection: Supports input, output truncate, and output append with strict file descriptor management to prevent leaks.
6. Background Process Management: Tracks active background jobs, prevents zombie processes using waitpid() with WNOHANG, and notifies the user upon job completion

# Known Limitations or Bugs
1. No Pipe Support: The shell does not currently support piping output between commands.
2. Quoted Arguments: The parser separates tokens strictly by whitespace. It does not support treating quoted strings with spaces as a single argument.
3. Hardcoded Limits: To guarantee memory safety without dynamic allocation, the shell restricts commands to a maximum of 255 arguments and limits background job tracking to 64 concurrent jobs.
4. Redirection Syntax: Redirection tokens must be separated by spaces.

# Design Decisions and Architecture Overview
The shell is built using a highly modular architecture emphasizing the separation of concerns, memory safety, and strict system call error handling.
1. Decoupled Parsing and Execution: Instead of raw string array manipulation during the execution phase, the shell uses structure.h. The parser.c module is solely responsible for parsing tokens and populating this struct. The external.c module blindly executes the instructions within the struct, adhering strictly to the Single Responsibility Principle.
2. Memory Management: The shell is designed to be completely memory-safe. It intentionally avoids dynamic memory allocation to eliminate the possibility of memory leaks. Tokens are processed in-place using strtok, and structures are stack-allocated.
3. File Descriptor Management: To prevent file descriptor leaks, dup2 is immediately followed by close() for any file opened during I/O redirection, guaranteeing the child process executes in a clean environment.
4. Job Control Module: Background process tracking is extracted into a dedicated jobs.c module. It maintains a stateful array of active PIDs and original command strings. At the start of every REPL iteration, the shell queries this list to clean up terminated child processes and print status updates, completely eliminating zombie processes