#include <string.h>
#include "parser.h"

int parse_input(char *input, char **args) {
    input[strcspn(input, "\n")] = 0;

    int i = 0;
    args[i] = strtok(input, " ");

    while (args[i] != NULL) {
        args[++i] = strtok(NULL, " ");
    }

    if (i > 0 && strcmp(args[i-1], "&") == 0) {
        args[i-1] = NULL;
        return 1;
    }

    return 0;
}