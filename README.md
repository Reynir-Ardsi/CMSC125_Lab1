# CMSC125_Laboratory1

# Complete Names of Group Members
Rainier RJ E. Espinal
Matthew F. Simpas

# Compilation and Usage Instructions
To compile the shell, use the following command:
1. gcc -o mysh mysh.c parser.c external.c builtin.c
2. ./mysh
3. Inside mysh, you can run built-in commands:
    mysh> cd /usr/bin
    mysh > pwd
    Or any external program:
    mysh > ls -la
    Or use i/o redirection:
    mysh > sort < unsorted . txt > sorted . txt
    Or append & to run it in the background:
    mysh > sleep 10 &

# List of Implemented Features
1. Interactive Command Loop
2. Built-in Commands
3. External Command Execution
4. I/O Redirection
5. Background Execution
6. Data Structures