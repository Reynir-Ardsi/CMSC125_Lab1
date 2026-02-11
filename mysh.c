#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

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

                if (dup2(fd, STDIN_FILENO) == -1) {
                    perror("mysh: dup2 error");
                    exit(EXIT_FAILURE);
                }
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

                if (dup2(fd, STDOUT_FILENO) == -1) {
                    perror("mysh: dup2 error");
                    exit(EXIT_FAILURE);
                }
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

                if (dup2(fd, STDOUT_FILENO) == -1) {
                    perror("mysh: dup2 error");
                    exit(EXIT_FAILURE);
                }
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
                fprintf(stderr, "mysh: command not found: %s\n", args[0]);
            }
        }
    }
}

int main() {
    char userInput[1024];
    char *args[64];

    while (1) {
        printf("mysh > ");
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