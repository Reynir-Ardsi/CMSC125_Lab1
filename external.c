#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "external.h"

pid_t execute_external(char **args, int background) {
    pid_t pid = fork();

    if (pid == 0) {
        int i = 0;
        int fd = -1;
        int first_redirect_pos = -1;

        while (args[i] != NULL) {
            if (strcmp(args[i], "<") == 0) {
                if (args[i+1] == NULL) {
                    fprintf(stderr, "mysh: expected filename after '<'\n");
                    exit(EXIT_FAILURE);
                }
                fd = open(args[i+1], O_RDONLY);
                if (fd == -1) {
                    perror("mysh: input file error");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
                if (first_redirect_pos == -1) first_redirect_pos = i;
                i++;
            }
            else if (strcmp(args[i], ">") == 0) {
                if (args[i+1] == NULL) {
                    fprintf(stderr, "mysh: expected filename after '>'\n");
                    exit(EXIT_FAILURE);
                }
                fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1) {
                    perror("mysh: output file error");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
                if (first_redirect_pos == -1) first_redirect_pos = i;
                i++;
            }
            else if (strcmp(args[i], ">>") == 0) {
                if (args[i+1] == NULL) {
                    fprintf(stderr, "mysh: expected filename after '>>'\n");
                    exit(EXIT_FAILURE);
                }
                fd = open(args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd == -1) {
                    perror("mysh: output file error");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
                if (first_redirect_pos == -1) first_redirect_pos = i;
                i++;
            }
            i++;
        }

        if (first_redirect_pos != -1) {
            args[first_redirect_pos] = NULL;
        }

        if (execvp(args[0], args) == -1) {
            perror("mysh");
        }

        exit(EXIT_FAILURE);
    }

    else if (pid < 0) {
        perror("mysh");
        return -1;
    }

    else {
        if (!background) {
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status)) {
                int exit_code = WEXITSTATUS(status);
                if (exit_code != 0) {
                    fprintf(stderr, "mysh: command not found: %s\n", args[0]);
                }
            }
            return 0;
        } else {
            return pid;
        }
    }
}