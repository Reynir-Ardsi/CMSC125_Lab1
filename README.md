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