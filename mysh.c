#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include "builtin.h"
#include "external.h"
#include "parser.h"
#include "structure.h"

int main() {
    char userInput[1024];
    int job_id = 0;

    while (1) {
        while (waitpid(-1, NULL, WNOHANG) > 0);

        printf("mysh > ");
        fflush(stdout);

        if (fgets(userInput, sizeof(userInput), stdin) == NULL) {
            printf("\n");
            break;
        }

        Command cmd;
        parse_input(userInput, &cmd);

        if (cmd.command == NULL)
            continue;

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
            
            if (cmd.background && pid > 0) {
                job_id++;
                printf("[%d] Started background job : ", job_id);
                for (int j = 0; cmd.args[j] != NULL; j++) {
                    printf("%s ", cmd.args[j]);
                }
                printf("( PID : %d)\n", pid);
            }
        }
    }

    return 0;
}