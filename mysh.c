#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

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

void execute_external(char **args) {
    pid_t pid = fork();

    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("mysh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("mysh");
    } else {
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            if (exit_code != 0) {
                fprintf(stderr, "[Process exited with status %d]\n", exit_code);
            }
        }
    }
}

int main() {
    char userInput[1024];
    char *args[64];

    while (1) {
        printf("mysh> ");
        fflush(stdout);

        if (fgets(userInput, sizeof(userInput), stdin) == NULL) {
            printf("\n");
            break;
        }

        userInput[strcspn(userInput, "\n")] = 0;

        int i = 0;
        args[i] = strtok(userInput, " ");
        while (args[i] != NULL) {
            args[++i] = strtok(NULL, " ");
        }

        if (args[0] == NULL) continue;

        if (strcmp(args[0], "exit") == 0) {
            while (waitpid(-1, NULL, WNOHANG) > 0);
            break;
        } 
        else if (strcmp(args[0], "cd") == 0) {
            cd(args[1]);
        } 
        else if (strcmp(args[0], "pwd") == 0) {
            pwd();
        } 
        else {
            execute_external(args);
        }
    }
    return 0;
}