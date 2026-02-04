#include <stdio.h>
#include <string.h>
#include <unistd.h>

//cd implementation
void cd(char *location){
    if (location == NULL || strlen(location) == 0) {
        printf("cd: path not specified\n");
        return;
    }
    if (chdir(location) != 0) {
        printf("Error changing directory to: %s\n", location);
    }else{
        printf("Changed directory to: %s\n", location);
    }
}

//pwd implementation
void pwd(){
    char cwd[1024]; //current working directory

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current directory: %s\n", cwd);
    } else {
        printf("Error getting current directory\n");
    }
}

int main(){
    char userInput[1024];

    while (1){
        printf("mysh> ");
        
        fgets(userInput, sizeof(userInput), stdin);
        userInput[strcspn(userInput, "\n")] = 0; // Remove newline character

        char *command = strtok(userInput, " ");
        char *location = strtok(NULL, " ");

        if (command == NULL) {
            continue; // prevent crashing on empty input
        }

        if (strcmp(command, "exit") == 0){
            break;
        }

        if(strcmp(command, "cd") == 0){
            cd(location);

        } else if(strcmp(command, "pwd") == 0){
            pwd();

        } else {
            printf("Command not found: \"%s\"\n", userInput);
        }
    }
    return 0;
}