#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define HISTORY_LIMIT 10    // The maximum number of commands in history
#define MAX_COMMAND_LENGTH 1024

char *history[HISTORY_LIMIT];  // Array to store command history
int history_count = 0;         // Counter to track the number of commands in history

// Function to add a command to history
void save_to_history(const char *command) {
    // Free the oldest command if history is full
    if (history_count == HISTORY_LIMIT) {
        free(history[0]);
        // Shift the commands to the left to make room for the new command
        for (int i = 1; i < HISTORY_LIMIT; i++) {
            history[i - 1] = history[i];
        }
        history_count--;  // Adjust the count after removing oldest entry
    }
    // Add the new command to history and increment the count
    history[history_count] = strdup(command);
    history_count++;
}

// Function to display the command history
void display_history() {
    printf("Command History:\n");
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i + 1, history[i]);
    }
}

// Function to retrieve and run a command from history by index
void run_from_history(int index) {
    if (index < 1 || index > history_count) {
        printf("No command found at this history index.\n");
        return;
    }
    printf("Repeating command: %s\n", history[index - 1]);
    system(history[index - 1]);  // Run the command
}

int main() {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        printf("MyShell> ");  // Display the shell prompt
        if (!fgets(command, MAX_COMMAND_LENGTH, stdin)) break;

        // Remove the newline character from the command
        command[strcspn(command, "\n")] = 0;

        // Check for history command (starting with '!')
        if (command[0] == '!' && strlen(command) > 1) {
            int index = atoi(&command[1]);  // Convert number after '!' to integer
            run_from_history(index);        // Run the specified command from history
            continue;
        }

        // Add the command to history
        save_to_history(command);

        // Execute the command by forking a child process
        if (fork() == 0) {
            char *args[] = {"/bin/sh", "-c", command, NULL};  // Execute command in shell
            execvp(args[0], args);
            perror("Execution error");
            exit(EXIT_FAILURE);
        } else {
            wait(NULL);  // Parent waits for the child to finish
        }
    }

    // Free allocated memory for history before exiting
    for (int i = 0; i < history_count; i++) {
        free(history[i]);
    }

    return 0;
}
