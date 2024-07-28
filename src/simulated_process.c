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
        printf("Processo simulado iniciado com PID: %d, ID: %d\n", getpid(), process_id);
        
        for (int i = 0; commands[i] != NULL; i++) {
            // TODO - Implementar a execução dos comandos
            printf("Executando comando: %s\n", commands[i]);
            sleep(1);
        }

        printf("Processo simulado com PID %d terminado\n", getpid());
        exit(EXIT_SUCCESS);
    } else {
        return pid; 
    }
}
