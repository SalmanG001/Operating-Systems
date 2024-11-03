#define HISTORY_SIZE 10
char *history[HISTORY_SIZE];
int history_count = 0;

void add_to_history(char *command) {
    if (history_count < HISTORY_SIZE) {
        history[history_count++] = strdup(command);
    } else {
        free(history[0]);
        for (int i = 1; i < HISTORY_SIZE; i++) history[i - 1] = history[i];
        history[HISTORY_SIZE - 1] = strdup(command);
    }
}

void execute_from_history(int index) {
    if (index >= 0 && index < history_count) {
        printf("Executing: %s\n", history[index]);
        execute_with_redirection(history[index]);  // Use Version 2 function
    } else {
        printf("Invalid history index\n");
    }
}
