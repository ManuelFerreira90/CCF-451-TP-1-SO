#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "../include/simulated_process.h"

int create_simulated_process(int process_id, const char *commands[]) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        // Código do processo simulado
        printf("Processo simulado iniciado com PID: %d, ID: %d\n", getpid(), process_id);
        
        // Simulação de execução dos comandos
        for (int i = 0; commands[i] != NULL; i++) {
            // TODO - Implementar a execução dos comandos
            printf("Executando comando: %s\n", commands[i]);
            sleep(1); // Simula a execução
        }

        printf("Processo simulado com PID %d terminado\n", getpid());
        exit(EXIT_SUCCESS);
    } else {
        // Processo pai continua
        return pid; // Retorna o PID do processo simulado
    }
}
