#include "../include/functions.h"

int main() {
    int num_processos;
    Processo* processos = ler_dados_arquivo("processos.txt", &num_processos);

    printf("FCFS:\n");
    escalonador_fcfs(processos, num_processos);
    calcular_metricas(processos, num_processos);

    printf("\nSJF:\n");
    escalonador_sjf(processos, num_processos);
    calcular_metricas(processos, num_processos);

    printf("\nRound-Robin (quantum = 2):\n");
    escalonador_rr(processos, num_processos, 2);
    calcular_metricas(processos, num_processos);

    free(processos);
    return 0;
}
