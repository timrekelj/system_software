#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGUMENTS 64

void parse(char *command, char *args[]) {
    if (command[0] == ' ') command = command + 1;

    char *token;
    int arg_index = 0;
    int command_len = strlen(command);

    token = command;
    for (int i = 0; i < command_len; i++) {
        if (command[i] == ' ') {            // If space, set character to \0 and add argument
            command[i] = '\0';
            args[arg_index++] = token;
            token = &command[i + 1];
            if (command[i + 1] == '\0')   // This happens if space is last character
                break;
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
    char *command00;
    char *command01;
    char *args00[MAX_ARGUMENTS];
    char *args01[MAX_ARGUMENTS];
    char *start_pipe;

    // Track even or odd commands
    int is_odd = 0;

    while (1) {
        // Show prompt
        printf(" > ");

        // Break when CTRL+D is pressed
        if (fgets(command, sizeof(command), stdin) == NULL) break;

        // Remove trailing newline
        command[strcspn(command, "\n")] = '\0';

        // Check if command includes pipe
        start_pipe = strstr(command, "==>");

        if (start_pipe != NULL) {
            *start_pipe = '\0';
            command01 = start_pipe + 3;
            command00 = command;

            // Choose type of parsing based on even or odd command
            if (is_odd = (is_odd + 1) % 2) {
                parse(command00, args00);
                parse(command01, args01);
            } else {
                parse_strtok(command00, args00);
                parse_strtok(command01, args01);
            }

            int des_p[2];
            if (pipe(des_p) == -1) {
                perror("Pipe failed");
                exit(1);
            }
            if (fork() == 0) { // First fork
                close(STDOUT_FILENO); // closing stdout
                dup(des_p[1]);        // replacing stdout with pipe write
                close(des_p[0]);      // closing pipe read
                close(des_p[1]);
                execvp(args00[0], args00);
                perror("execvp of first command failed");
                exit(1);
            } if (fork() == 0) { // 2nd fork
                close(STDIN_FILENO); // closing stdin
                dup(des_p[0]);       // replacing stdin with pipe read
                close(des_p[1]);     // closing pipe write
                close(des_p[0]);

                execvp(args01[0], args01);
                perror("execvp of second command failed");
                exit(1);
            }

            close(des_p[0]);
            close(des_p[1]);
            wait(0);
            wait(0);

        } else {
            // Choose type of parsing based on even or odd command
            if (is_odd = (is_odd + 1) % 2) parse(command, args00);
            else parse_strtok(command, args00);

            // Create a child process
            pid_t pid = fork();

            if (pid == -1) {       // Error
                perror("Error with fork()!");
                exit(EXIT_FAILURE);
            } else if (pid == 0) { // Child process
                execvp(args00[0], args00);

                perror("Error with execvp()!"); // This will run only if execvp() fails
                exit(EXIT_FAILURE);
            } else {               // Parent process
                int status;
                waitpid(pid, &status, 0);
            }
        }
    }

    printf("\nExiting\n");

    return 0;
}