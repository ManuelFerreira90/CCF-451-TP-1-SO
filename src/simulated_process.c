#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "../include/simulated_process.h"

#define MAX_PROCESSES 10
#define MAX_VARS 10

typedef struct {
    int id;
    int num_vars;
    int vars[MAX_VARS];
    int pc;
    int state; // 0 = ready, 1 = blocked, 2 = terminated
} Process;

Process process_table[MAX_PROCESSES];
int process_count = 0;

void handle_N(int process_id, int n) {
    process_table[process_id].id = process_id;
    process_table[process_id].num_vars = n;
    memset(process_table[process_id].vars, 0, n * sizeof(int));
    process_table[process_id].state = 0; // ready
    printf("Processo %d: Número de variáveis definido para %d\n", process_id, n);
}

void handle_D(int process_id, int x) {
    if (x >= process_table[process_id].num_vars) {
        fprintf(stderr, "Erro: índice da variável fora dos limites\n");
        return;
    }
    process_table[process_id].vars[x] = 0;
    printf("Processo %d: Variável %d redefinida para 0\n", process_id, x);
}

void handle_V(int process_id, int x, int value) {
    if (x >= process_table[process_id].num_vars) {
        fprintf(stderr, "Erro: índice da variável fora dos limites\n");
        return;
    }
    process_table[process_id].vars[x] = value;
    printf("Processo %d: Variável %d definida para %d\n", process_id, x, value);
}

void handle_A(int process_id, int x, int value) {
    if (x >= process_table[process_id].num_vars) {
        fprintf(stderr, "Erro: índice da variável fora dos limites\n");
        return;
    }
    process_table[process_id].vars[x] += value;
    printf("Processo %d: Variável %d incrementada em %d\n", process_id, x, value);
}

void handle_S(int process_id, int x, int value) {
    if (x >= process_table[process_id].num_vars) {
        fprintf(stderr, "Erro: índice da variável fora dos limites\n");
        return;
    }
    process_table[process_id].vars[x] -= value;
    printf("Processo %d: Variável %d decrementada em %d\n", process_id, x, value);
}

void handle_F(int process_id) {
    process_table[process_id].state = 2; // Terminated
    printf("Processo %d: Terminado\n", process_id);
}

void handle_R(int process_id, char *file) {
    printf("Processo %d: Lendo arquivo %s\n", process_id, file);
    // Implementação da leitura do arquivo
}

void handle_T(int process_id) {
    printf("Processo %d: T\n", process_id);
    // Implementação do comando T
}

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
