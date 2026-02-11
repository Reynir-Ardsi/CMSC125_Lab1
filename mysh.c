#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include "builtin.h"
#include "external.h"
#include "parser.h"

int main() {
    char userInput[1024];
    char *args[64];
    int job_id = 0;

    while (1) {
        while (waitpid(-1, NULL, WNOHANG) > 0);

        printf("mysh > ");
        fflush(stdout);

        if (fgets(userInput, sizeof(userInput), stdin) == NULL) {
            printf("\n");
            break;
        }

        int is_bg = parse_input(userInput, args);

        if (args[0] == NULL)
            continue;

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
            pid_t pid = execute_external(args, is_bg);
            
            if (is_bg && pid > 0) {
                job_id++;
                printf("[%d] Started background job : ", job_id);
                
                for (int j = 0; args[j] != NULL; j++) {
                    printf("%s ", args[j]);
                }
                
                printf("( PID : %d)\n", pid);
            }
        }
    }

    return 0;
}