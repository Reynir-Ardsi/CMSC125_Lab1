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
1. Interactive Command Loop
2. Built-in Commands
3. External Command Execution
4. I/O Redirection
5. Background Execution
6. Data Structures