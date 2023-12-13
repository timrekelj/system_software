#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGUMENTS 64

void parse(char *command, char *args[]) {
    printf("Odd command, calling parse\n");

    char *token;
    int arg_index = 0;
    int command_len = strlen(command);

    // Split command into arguments manually
    token = command;
    for (int i = 0; i < command_len; i++) {
        if (command[i] == ' ') { // If space, set character to \0 and add argument
            if (command[i + 1] == '\0') { // This happens if space is last character
                args[arg_index++] = NULL;
                break;
            }
            command[i] = '\0';
            args[arg_index++] = token;
            token = &command[i + 1];

        } else if (arg_index >= MAX_ARGUMENTS - 1) {
            args[arg_index++] = NULL;
            break;

        } else if (command[i + 1] == '\0') {
            args[arg_index++] = token;
            token = NULL;
        }
    } 
    args[arg_index] = NULL;
}

void parse_strtok(char *command, char *args[]) {
    printf("Even command, calling strtok\n");

    char *token;
    int arg_index = 0;

    // Split command into arguments with strtok
    token = strtok(command, " \n");
    for (int i = 0; i < MAX_ARGUMENTS; i++) {
        if (token == NULL) {
            args[arg_index] = NULL;
            break;
        }
        args[arg_index++] = token;
        token = strtok(NULL, " \n");
    }

}

int main() {
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGUMENTS];

    // Track even or odd commands
    int is_odd = 0;


    while (1) {
        // Show prompt
        printf(" > ");

        // Break when CTRL+D is pressed
        if (fgets(command, sizeof(command), stdin) == NULL) break;

        // Remove trailing newline
        command[strcspn(command, "\n")] = '\0';

        // Choose type of parsing based on even or odd command
        if (is_odd = (is_odd + 1) % 2) parse(command, args);
        else parse_strtok(command, args);

        // Create a child process
        pid_t pid = fork();

        if (pid == -1) {       // Error
            perror("Error with fork()!");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Child process
            execvp(args[0], args);

            perror("Error with execvp()!"); // This will run only if execvp() fails
            exit(EXIT_FAILURE);
        } else {               // Parent process
            int status;
            waitpid(pid, &status, 0);
        }
    }

    printf("\nExiting\n");

    return 0;
}