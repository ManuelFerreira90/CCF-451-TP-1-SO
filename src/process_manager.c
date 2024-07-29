#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process_manager.h"
#include "../include/utils.h"
#include "../include/simulated_process.h"

#define MAX_CMD_LEN 256

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
            handle_V(process_id, x, n);
            break;
        case 'A':
            sscanf(cmd, "A %d %d %d", &process_id, &x, &n);
            handle_A(process_id, x, n);
            break;
        case 'S':
            sscanf(cmd, "S %d %d %d", &process_id, &x, &n);
            handle_S(process_id, x, n);
            break;
        case 'F':
            sscanf(cmd, "F %d", &process_id);
            handle_F(process_id);
            break;
        case 'R':
            sscanf(cmd, "R %d %s", &process_id, file);
            handle_R(process_id, file);
            break;
        case 'T':
            sscanf(cmd, "T %d", &process_id);
            handle_T(process_id);
            break;
        default:
            fprintf(stderr, "Comando desconhecido: %s", cmd);
    }
}

int main() {
    char cmd[MAX_CMD_LEN];
    while (fgets(cmd, sizeof(cmd), stdin)) {
        handle_command(cmd);
    }
    return 0;
}
