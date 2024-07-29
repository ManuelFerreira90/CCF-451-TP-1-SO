#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/control_process.h"
#include "../include/utils.h"

#define MAX_CMD_LEN 256
#define PIPE_READ 0
#define PIPE_WRITE 1

int start_control_process()
{
    int fd[2];
    pid_t pid;

    if (pipe(fd) == -1)
    {
        perror("pipe");
        return -1;
    }

    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return -1;
    }

    if (pid == 0)
    {
        close(fd[PIPE_WRITE]);
        dup2(fd[PIPE_READ], STDIN_FILENO); 
        close(fd[PIPE_READ]);

        execl("./bin/process_manager", "process_manager", NULL); 
        perror("execl"); 
        return -1;
    }
    else 
    {
        close(fd[PIPE_READ]);

        FILE *input = fopen("./entry/input.txt", "r");
        if (!input)
        {
            perror("fopen");
            return -1;
        }

        char line[MAX_CMD_LEN];
        while (fgets(line, sizeof(line), input))
        {
            write(fd[PIPE_WRITE], line, strlen(line)); 
        }

        fclose(input);
        close(fd[PIPE_WRITE]);
        wait(NULL);
    }

    return 0;
}
