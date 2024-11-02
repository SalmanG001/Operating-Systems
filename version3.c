#include <sys/types.h>
#include <signal.h>

void execute_background(char *command) {
    char *args[100];
    int i = 0;
    args[i] = strtok(command, " ");
    while (args[i] != NULL) args[++i] = strtok(NULL, " ");
    
    int is_background = 0;
    if (strcmp(args[i-1], "&") == 0) {
        args[i-1] = NULL;
        is_background = 1;
    }
    
    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror("Execution failed");
        exit(EXIT_FAILURE);
    }
    if (!is_background) wait(NULL);
}
