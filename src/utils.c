#include <stdio.h>
#include <stdlib.h>
#include "../include/utils.h"

void handle_error(const char *message) {
    FILE *log_file = fopen("error.log", "a");
    if (log_file) {
        fprintf(log_file, "%s: ", message);
        perror(NULL);
        fclose(log_file);
    } else {
        fprintf(stderr, "Failed to open log file.\n");
    }
    exit(EXIT_FAILURE);
}
