#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "../include/simulated_process.h"

#define MAX_PROCESSES 10
#define MAX_VARS 10

typedef struct {
    int num_vars;
    int vars[MAX_VARS];
    int pc;
    int state; // 0 = ready, 1 = blocked, 2 = terminated
} Process;

Process process_table[MAX_PROCESSES];
int process_count = 0;
int current_process = -1;

void switch_to_process(int process_id) {
    if (process_id < 0 || process_id >= process_count) {
        fprintf(stderr, "Erro: ID do processo inválido\n");
        return;
    }
    current_process = process_id;
}

void handle_N(int n) {
    if (process_count >= MAX_PROCESSES) {
        fprintf(stderr, "Erro: número máximo de processos atingido\n");
        return;
    }
    process_table[process_count].num_vars = n;
    memset(process_table[process_count].vars, 0, n * sizeof(int));
    process_table[process_count].state = 0; // ready
    printf("Processo %d: Número de variáveis definido para %d\n", process_count, n);
    switch_to_process(process_count);
    process_count++;
}

void handle_D(int x) {
    if (current_process == -1 || x < 0 || x >= process_table[current_process].num_vars) {
        fprintf(stderr, "Erro: índice da variável fora dos limites\n");
        return;
    }
    process_table[current_process].vars[x] = 0;
    printf("Processo %d: Variável %d redefinida para 0\n", current_process, x);
}

void handle_V(int x, int value) {
    if (current_process == -1 || x < 0 || x >= process_table[current_process].num_vars) {
        fprintf(stderr, "Erro: índice da variável fora dos limites\n");
        return;
    }
    process_table[current_process].vars[x] = value;
    printf("Processo %d: Variável %d definida para %d\n", current_process, x, value);
}

void handle_A(int x, int value) {
    if (current_process == -1 || x < 0 || x >= process_table[current_process].num_vars) {
        fprintf(stderr, "Erro: índice da variável fora dos limites\n");
        return;
    }
    process_table[current_process].vars[x] += value;
    printf("Processo %d: Variável %d incrementada em %d\n", current_process, x, value);
}

void handle_S(int x, int value) {
    if (current_process == -1 || x < 0 || x >= process_table[current_process].num_vars) {
        fprintf(stderr, "Erro: índice da variável fora dos limites\n");
        return;
    }
    process_table[current_process].vars[x] -= value;
    printf("Processo %d: Variável %d decrementada em %d\n", current_process, x, value);
}

void handle_B(int n) {
    if (current_process == -1) {
        fprintf(stderr, "Erro: nenhum processo atual\n");
        return;
    }
    process_table[current_process].state = 1; // blocked
    printf("Processo %d: Bloqueado por %d unidades de tempo\n", current_process, n);
    // Implementar lógica de bloqueio aqui
}

void handle_F() {
    if (process_count >= MAX_PROCESSES) {
        fprintf(stderr, "Erro: número máximo de processos atingido\n");
        return;
    }
    process_table[process_count] = process_table[current_process];
    process_table[current_process].state = 0; // ready
    printf("Processo %d: Novo processo criado\n", process_count);
    switch_to_process(process_count);
    process_count++;
}

void handle_R(char *file) {
    if (current_process == -1) {
        fprintf(stderr, "Erro: nenhum processo atual\n");
        return;
    }
    printf("Processo %d: Lendo arquivo %s\n", current_process, file);
    // Implementar a leitura do arquivo aqui
}

void handle_T() {
    if (current_process == -1) {
        fprintf(stderr, "Erro: nenhum processo atual\n");
        return;
    }
    process_table[current_process].state = 2; // terminated
    printf("Processo %d: Terminado\n", current_process);
}

int create_simulated_process(const char *commands[]) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        printf("Processo simulado iniciado com PID: %d\n", getpid());

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
    //
}
