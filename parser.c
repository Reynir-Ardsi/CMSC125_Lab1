#include <string.h>
#include <stddef.h>
#include <stdio.h>
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

    if (input == NULL) {
        return;
    }

    int arg_count = 0;
    char *token = strtok(input, " \t\r\n");

    if (token == NULL) {
        return; 
    }

    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " \t\r\n");
            if (token != NULL) {
                cmd->input_file = token;
            } else {
                fprintf(stderr, "mysh: expected filename after '<'\n");
            }
        } 
        else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " \t\r\n");
            if (token != NULL) {
                cmd->output_file = token;
                cmd->append = false;
            } else {
                fprintf(stderr, "mysh: expected filename after '>'\n");
            }
        } 
        else if (strcmp(token, ">>") == 0) {
            token = strtok(NULL, " \t\r\n");
            if (token != NULL) {
                cmd->output_file = token;
                cmd->append = true;
            } else {
                fprintf(stderr, "mysh: expected filename after '>>'\n");
            }
        } 
        else if (strcmp(token, "&") == 0) {
            cmd->background = true;
        } 
        else {
            if (arg_count < 255) {
                if (cmd->command == NULL) {
                    cmd->command = token;
                }
                cmd->args[arg_count++] = token;
            } else {
                fprintf(stderr, "mysh: warning: too many arguments, truncating\n");
                break;
            }
        }
        token = strtok(NULL, " \t\r\n");
    }
}