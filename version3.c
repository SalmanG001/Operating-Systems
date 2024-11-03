#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void execute_background(char *command) {
    char *args[100];      // Array to hold the command and arguments
    int i = 0;

    // Tokenize the command input to separate it into command and arguments
    args[i] = strtok(command, " ");
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " ");
    }

    // Check if the command should run in the background (ends with "&")
    int is_background = 0;
    if (i > 0 && strcmp(args[i - 1], "&") == 0) {
        args[i - 1] = NULL;   // Remove "&" from arguments
        is_background = 1;    // Mark as background process
    }

    // Create a new process to execute the command
    pid_t pid = fork();  // Fork creates a child process
    if (pid == 0) {
        // In child process: execute the command
        execvp(args[0], args);
        perror("Execution failed");  // Print error if command fails
        exit(EXIT_FAILURE);          // Exit if execution fails
    } 
    else {
        // In parent process
        if (!is_background) {
            wait(NULL);  // Wait for child process to finish if not background
        }
    }
}
