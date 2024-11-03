#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 100
#define MAX_JOBS 10

// Structure to keep track of background jobs
typedef struct {
    pid_t pid;
    char command[MAX_INPUT];
} Job;

Job jobs[MAX_JOBS];
int job_count = 0;

// Function prototypes
void execute_command(char *command);
int is_builtin_command(char **args);
void execute_builtin(char **args);
void change_directory(char *path);
void list_jobs();
void kill_job(int job_num);
void show_help();
void add_job(pid_t pid, char *command);
void remove_job(pid_t pid);

int main() {
    char command[MAX_INPUT];
    while (1) {
        printf("MyShell> ");  // Display the shell prompt
        if (!fgets(command, MAX_INPUT, stdin)) break;

        // Remove newline character from command
        command[strcspn(command, "\n")] = 0;

        // Execute command
        execute_command(command);
    }
    return 0;
}

// Main command execution function
void execute_command(char *command) {
    char *args[MAX_ARGS];
    int i = 0;

    // Tokenize command into arguments
    args[i] = strtok(command, " ");
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " ");
    }

    // Check if command is a built-in command
    if (args[0] == NULL) return;  // Ignore empty input
    if (is_builtin_command(args)) {
        execute_builtin(args);  // Execute the built-in command
    } else {
        // For other commands, fork and execute
        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args);  // Execute external command
            perror("Command execution failed");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            if (strcmp(args[i - 1], "&") == 0) {  // Check for background process
                args[i - 1] = NULL;  // Remove "&" from arguments
                add_job(pid, command);  // Add job to background jobs list
            } else {
                wait(NULL);  // Wait for child process to finish
            }
        } else {
            perror("Fork failed");
        }
    }
}

// Check if the command is a built-in command
int is_builtin_command(char **args) {
    return strcmp(args[0], "cd") == 0 || strcmp(args[0], "exit") == 0 ||
           strcmp(args[0], "jobs") == 0 || strcmp(args[0], "kill") == 0 ||
           strcmp(args[0], "help") == 0;
}

// Execute the built-in command
void execute_builtin(char **args) {
    if (strcmp(args[0], "cd") == 0) {
        change_directory(args[1]);
    } else if (strcmp(args[0], "exit") == 0) {
        printf("Exiting shell...\n");
        exit(0);
    } else if (strcmp(args[0], "jobs") == 0) {
        list_jobs();
    } else if (strcmp(args[0], "kill") == 0) {
        if (args[1] == NULL) {
            printf("Usage: kill <job_number>\n");
        } else {
            kill_job(atoi(args[1]));
        }
    } else if (strcmp(args[0], "help") == 0) {
        show_help();
    }
}

// Change directory command
void change_directory(char *path) {
    if (path == NULL) {
        printf("Usage: cd <directory>\n");
    } else if (chdir(path) != 0) {
        perror("cd failed");
    }
}

// List background jobs
void list_jobs() {
    printf("Background jobs:\n");
    for (int i = 0; i < job_count; i++) {
        printf("[%d] %d %s\n", i + 1, jobs[i].pid, jobs[i].command);
    }
}

// Kill a specific background job
void kill_job(int job_num) {
    if (job_num <= 0 || job_num > job_count) {
        printf("Invalid job number.\n");
        return;
    }
    pid_t pid = jobs[job_num - 1].pid;
    if (kill(pid, SIGKILL) == 0) {
        printf("Job [%d] with PID %d killed.\n", job_num, pid);
        remove_job(pid);
    } else {
        perror("Failed to kill job");
    }
}

// Display help information for built-in commands
void show_help() {
    printf("Available commands:\n");
    printf("cd <directory> - Change the current directory\n");
    printf("exit - Exit the shell\n");
    printf("jobs - List all background jobs\n");
    printf("kill <job_number> - Kill a background job by its number\n");
    printf("help - Show this help message\n");
}

// Add a background job to the jobs list
void add_job(pid_t pid, char *command) {
    if (job_count < MAX_JOBS) {
        jobs[job_count].pid = pid;
        strncpy(jobs[job_count].command, command, MAX_INPUT - 1);
        job_count++;
    } else {
        printf("Job limit reached. Cannot add more background jobs.\n");
    }
}

// Remove a job from the jobs list by PID
void remove_job(pid_t pid) {
    for (int i = 0; i < job_count; i++) {
        if (jobs[i].pid == pid) {
            for (int j = i; j < job_count - 1; j++) {
                jobs[j] = jobs[j + 1];
            }
            job_count--;
            break;
        }
    }
}
