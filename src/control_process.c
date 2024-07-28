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

    // Criação do pipe
    if (pipe(fd) == -1)
    {
        perror("pipe");
        return -1;
    }

    // Criação do processo
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return -1;
    }

    if (pid == 0) // Processo filho (Gerenciador de Processos)
    {
        close(fd[PIPE_WRITE]); // Fecha a extremidade de escrita do pipe
        dup2(fd[PIPE_READ], STDIN_FILENO); // Redireciona a entrada padrão para ler do pipe
        close(fd[PIPE_READ]); // Fecha a extremidade de leitura do pipe

        // Executa o gerenciador de processos
        execl("./process_manager", "process_manager", NULL); // Atualiza o caminho
        perror("execl"); // Se execl falhar
        return -1;
    }
    else // Processo pai (Controle)
    {
        close(fd[PIPE_READ]); // Fecha a extremidade de leitura do pipe

        FILE *input = fopen("../entry/input.txt", "r");
        if (!input)
        {
            perror("fopen");
            return -1;
        }

        char line[MAX_CMD_LEN];
        while (fgets(line, sizeof(line), input))
        {
            write(fd[PIPE_WRITE], line, strlen(line)); // Envia comandos pelo pipe
        }

        fclose(input);
        close(fd[PIPE_WRITE]); // Fecha a extremidade de escrita do pipe
        wait(NULL); // Espera o processo filho terminar
    }

    return 0;
}
