#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process_manager.h"
#include "../include/utils.h"

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
    process_table[process_id].num_vars = n;
    memset(process_table[process_id].vars, 0, n * sizeof(int));
}

void handle_D(int process_id, int x) {
    if (x >= process_table[process_id].num_vars) {
        fprintf(stderr, "Error: variable index out of bounds\n");
        return;
    }
    process_table[process_id].vars[x] = 0;
}

void handle_F(int process_id) {
    if (process_count >= MAX_PROCESSES) {
        fprintf(stderr, "Error: maximum number of processes reached\n");
        return;
    }
    Process *parent = &process_table[process_id];
    Process *new_process = &process_table[process_count++];
    
    // Copiar o estado do processo pai
    new_process->id = process_count; // Novo ID para o processo filho
    new_process->num_vars = parent->num_vars;
    memcpy(new_process->vars, parent->vars, parent->num_vars * sizeof(int));
    new_process->pc = parent->pc; // Continua a partir da próxima instrução
    new_process->state = 0; // Pronto
}

void handle_R(int process_id) {
    // TODO - Implementar a lógica para carregar o programa do arquivo
    process_table[process_id].pc = 0; // Reinicia o contador de programa
}

void handle_command(char *cmd) {
    int process_id, x, n;
    char file[256];

    switch (cmd[0]) {
        case 'N':
            sscanf(cmd, "N %d %d", &process_id, &n);
            handle_N(process_id, n);
            break;
        case 'D':
            sscanf(cmd, "D %d %d", &process_id, &x);
            handle_D(process_id, x);
            break;
        case 'V':
            sscanf(cmd, "V %d %d %d", &process_id, &x, &n);
            process_table[process_id].vars[x] = n;
            break;
        case 'A':
            sscanf(cmd, "A %d %d %d", &process_id, &x, &n);
            process_table[process_id].vars[x] += n;
            break;
        case 'S':
            sscanf(cmd, "S %d %d %d", &process_id, &x, &n);
            process_table[process_id].vars[x] -= n;
            break;
        case 'F':
            sscanf(cmd, "F %d", &process_id);
            handle_F(process_id);
            break;
        case 'R':
            sscanf(cmd, "R %d %s", &process_id, file);
            handle_R(process_id);
            break;
        case 'T':
            sscanf(cmd, "T %d", &process_id);
            process_table[process_id].state = 2; // Terminated
            break;
        default:
            fprintf(stderr, "Unknown command: %s\n", cmd);
            break;
    }
}

