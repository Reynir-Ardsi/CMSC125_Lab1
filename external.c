#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "external.h"
#include "structure.h"

pid_t execute_external(Command *cmd) {
    pid_t pid = fork();

    if (pid == 0) {
        if (cmd->input_file != NULL) {
            int fd = open(cmd->input_file, O_RDONLY);
            if (fd == -1) {
                perror("mysh: input file error");
                exit(EXIT_FAILURE);
            }
            if (dup2(fd, STDIN_FILENO) == -1) {
                perror("mysh: dup2 input error");
                exit(EXIT_FAILURE);
            }
            if (close(fd) == -1) {
                perror("mysh: close input error");
            }
        }

        if (cmd->output_file != NULL) {
            int flags = O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);
            int fd = open(cmd->output_file, flags, 0644);
            if (fd == -1) {
                perror("mysh: output file error");
                exit(EXIT_FAILURE);
            }
            if (dup2(fd, STDOUT_FILENO) == -1) {
                perror("mysh: dup2 output error");
                exit(EXIT_FAILURE);
            }
            if (close(fd) == -1) {
                perror("mysh: close output error");
            }
        }

        if (execvp(cmd->command, cmd->args) == -1) {
            exit(127);
        }
    }
    else if (pid < 0) {
        perror("mysh: fork failed");
        return -1;
    }
    else {
        if (!cmd->background) {
            int status;
            if (waitpid(pid, &status, 0) == -1) {
                perror("mysh: waitpid error");
            }

            if (WIFEXITED(status) && WEXITSTATUS(status) == 127) {
                fprintf(stderr, "mysh: command not found: %s\n", cmd->command);
            }
            return 0;
        } else {
            return pid;
        }
    }
    return 0;
}