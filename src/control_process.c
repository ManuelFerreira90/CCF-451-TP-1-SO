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

int start_control_process(const char *input_file)
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
        // Processo filho: processo gerenciador de processos
        close(fd[PIPE_WRITE]); // Fechar o lado de escrita no processo filho
        dup2(fd[PIPE_READ], STDIN_FILENO); // Redirecionar stdin para ler do pipe
        close(fd[PIPE_READ]);

        execl("./bin/process_manager", "process_manager", NULL);
        perror("execl");
        return -1;
    }
    else 
    {
        // Processo pai: processo de controle
        close(fd[PIPE_READ]); // Fechar o lado de leitura no processo pai

        FILE *input = NULL;
        if (input_file) 
        {
            input = fopen("./entry/input.txt", "r");
            if (!input)
            {
                perror("fopen");
                return -1;
            }
        }
        else
        {
            input = stdin;
        }

        char line[MAX_CMD_LEN];
        while (fgets(line, sizeof(line), input))
        {
            write(fd[PIPE_WRITE], line, strlen(line));
            // Verificar se o comando é 'M' para finalizar
            if (line[0] == 'M') 
            {
                break;
            }
        }

        if (input != stdin) 
        {
            fclose(input);
        }
        
        close(fd[PIPE_WRITE]);
        wait(NULL);
    }

    return 0;
}
