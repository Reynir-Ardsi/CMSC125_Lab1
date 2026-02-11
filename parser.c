#include <string.h>
#include "parser.h"

void parse_input(char *input, char **args) {
    // Remove newline
    input[strcspn(input, "\n")] = 0;

    int i = 0;
    args[i] = strtok(input, " ");

    while (args[i] != NULL) {
        args[++i] = strtok(NULL, " ");
    }
}
