#include <stdio.h>
#include <unistd.h>
#include "builtin.h"

void cd(char *location) {
    if (location == NULL) {
        fprintf(stderr, "mysh: expected argument to \"cd\"\n");
    } else if (chdir(location) != 0) {
        perror("mysh");
    }
}

void pwd() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("mysh");
    }
}

void shell_status() {
    printf("Shell Process Info:\n");
    printf("Process ID (PID): %d\n", getpid());
    printf("Parent PID (PPID): %d\n", getppid());
}