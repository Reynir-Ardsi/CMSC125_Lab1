#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "builtin.h"
#include "external.h"
#include "parser.h"
#include "structure.h"
#include "jobs.h"

int main() {
    char *userInput = NULL;
    size_t bufsize = 0;
    int job_id = 0;

    init_jobs();

    while (1) {
        cleanup_background_jobs();

        printf("mysh > ");
        fflush(stdout);

        if (getline(&userInput, &bufsize, stdin) == -1) {
            printf("\n");
            break;
        }

        Command cmd;
        parse_input(userInput, &cmd);

        if (cmd.command == NULL) {
            continue;
        }

        if (strcmp(cmd.command, "exit") == 0) {
            while (wait(NULL) > 0);
            break;
        }
        else if (strcmp(cmd.command, "cd") == 0) {
            cd(cmd.args[1]);
        }
        else if (strcmp(cmd.command, "pwd") == 0) {
            pwd();
        }
        else if (strcmp(cmd.command, "status") == 0) {
            shell_status();
        }
        
        else {
            pid_t pid = execute_external(&cmd);

            if (pid < 0) {
                fprintf(stderr, "mysh: command execution failed\n");
            }
            
            if (cmd.background && pid > 0) {
                job_id++;
                
                char cmd_str[1024];
                cmd_str[0] = '\0';

                for (int j = 0; cmd.args[j] != NULL; j++) {
                    if (strlen(cmd_str) + strlen(cmd.args[j]) + 2 < sizeof(cmd_str)) {
                        strcat(cmd_str, cmd.args[j]);

                        if (cmd.args[j+1] != NULL) {
                            strcat(cmd_str, " ");
                        }
                    } else {
                        break;
                    }
                }

                
                printf("[%d] Started : %s (PID : %d)\n", job_id, cmd_str, pid);
                
                add_background_job(job_id, pid, cmd_str);
            }
        }
        free_command(&cmd);
    }
    free(userInput);
    return 0;
}