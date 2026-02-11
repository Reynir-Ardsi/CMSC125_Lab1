#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include "builtin.h"
#include "external.h"
#include "parser.h"

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

        parse_input(userInput, args);

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
            execute_external(args);
        }
    }

    return 0;
}
