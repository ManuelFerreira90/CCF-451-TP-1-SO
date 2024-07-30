#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/process_manager.h"
#include "../include/utils.h"
#include "../include/simulated_process.h"

#define MAX_CMD_LEN 256

void handle_command(char *cmd) {
    int x, n;
    char file[256];

    switch (cmd[0]) {
        case 'N':
            sscanf(cmd, "N %d", &n);
            handle_N(n);
            break;
        case 'D':
            sscanf(cmd, "D %d", &x);
            handle_D(x);
            break;
        case 'V':
            sscanf(cmd, "V %d %d", &x, &n);
            handle_V(x, n);
            break;
        case 'A':
            sscanf(cmd, "A %d %d", &x, &n);
            handle_A(x, n);
            break;
        case 'S':
            sscanf(cmd, "S %d %d", &x, &n);
            handle_S(x, n);
            break;
        case 'B':
            sscanf(cmd, "B %d", &n);
            handle_B(n);
            break;
        case 'F':
            handle_F();
            break;
        case 'R':
            sscanf(cmd, "R %s", file);
            handle_R(file);
            break;
        case 'T':
            handle_T();
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
