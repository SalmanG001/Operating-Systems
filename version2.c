#include <fcntl.h>
#include <unistd.h>

void execute_with_redirection(char *command) {
    char *args[100];
    int i = 0, in_fd, out_fd;
    int redir_in = 0, redir_out = 0;

    args[i] = strtok(command, " ");
    while (args[i] != NULL) args[++i] = strtok(NULL, " ");

    for (int j = 0; j < i; j++) {
        if (strcmp(args[j], "<") == 0) {
            redir_in = 1;
            in_fd = open(args[j + 1], O_RDONLY);
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
            args[j] = NULL;
        }
        if (strcmp(args[j], ">") == 0) {
            redir_out = 1;
            out_fd = open(args[j + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
            args[j] = NULL;
        }
    }

    if (fork() == 0) {
        execvp(args[0], args);
        perror("Execution failed");
        exit(EXIT_FAILURE);
    }
    wait(NULL);
}
