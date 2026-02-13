#include <string.h>
#include <stddef.h>
#include "parser.h"
#include "structure.h"

void parse_input(char *input, Command *cmd) {
    cmd->command = NULL;
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append = false;
    cmd->background = false;
    for (int i = 0; i < 256; i++) {
        cmd->args[i] = NULL;
    }

    input[strcspn(input, "\n")] = 0;

    int arg_count = 0;
    char *token = strtok(input, " ");

    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " ");
            cmd->input_file = token;
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " ");
            cmd->output_file = token;
            cmd->append = false;
        } else if (strcmp(token, ">>") == 0) {
            token = strtok(NULL, " ");
            cmd->output_file = token;
            cmd->append = true;
        } else if (strcmp(token, "&") == 0) {
            cmd->background = true;
        } else {
            if (cmd->command == NULL) {
                cmd->command = token;
            }
            cmd->args[arg_count++] = token;
        }
        token = strtok(NULL, " ");
    }
}