#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 1024

void shell_loop() {
    char command[MAX_INPUT];
    char *args[100];
    while (1) {
        printf("PUCITshell:- ");  
        if (!fgets(command, MAX_INPUT, stdin)) break;

        command[strcspn(command, "\n")] = 0;

        int i = 0;
        args[i] = strtok(command, " ");
        while (args[i] != NULL) args[++i] = strtok(NULL, " ");

        if (args[0] == NULL) continue;

        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
        } else if (pid == 0) {
            execvp(args[0], args);
            perror("Execution failed of PUCIT SHELL");
            exit(EXIT_FAILURE);
        } else {
            wait(NULL); 
        }
    }
}

int main() {
    shell_loop();
    return 0;
}
