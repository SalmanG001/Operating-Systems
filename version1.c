#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 1024  

int main() {
    char command[MAX_INPUT];  // To store the user's command
    char *args[100];          // To store the command and arguments separately

    while (1) {  // Infinite loop to keep the shell running
        printf("MySimpleShell:- ");  // Display the prompt

        // Read input from the user
        if (!fgets(command, MAX_INPUT, stdin)) break;  // Exit on Ctrl+D

        // Remove the newline character at the end of the command
        command[strcspn(command, "\n")] = 0;

        // Split the command into arguments
        int i = 0;
        args[i] = strtok(command, " ");  // Tokenize based on spaces
        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " ");
        }

        // Check for the "exit" command to quit the shell
        if (args[0] != NULL && strcmp(args[0], "exit") == 0) {
            printf("Exiting shell...\n");
            break;
        }

        // Create a new process to execute the command
        pid_t pid = fork();  // Fork a child process
        if (pid < 0) {
            perror("Error in forking");  // Print an error if fork fails
        } else if (pid == 0) {
            // Child process: execute the command
            execvp(args[0], args);
            perror("Command not Found");  // Print error if exec fails
            exit(EXIT_FAILURE);           // Exit child process on failure
        } else {
            // Parent process: wait for the child to finish
            wait(NULL);
        }
    }

    return 0;
}



