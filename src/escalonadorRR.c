#include "../include/functions.h"


void escalonador_rr(Processo* processos, int num_processos, int quantum) {
    int tempo_atual = 0;
    int* tempo_restante = (int*)malloc(num_processos * sizeof(int));
    int* tempo_chegada = (int*)malloc(num_processos * sizeof(int));
    for (int i = 0; i < num_processos; i++) {
        tempo_restante[i] = processos[i].tempo_execucao;
        tempo_chegada[i] = processos[i].tempo_chegada;
    }

    int processos_restantes = num_processos;

    while (processos_restantes > 0) {
        for (int i = 0; i < num_processos; i++) {
            if (tempo_restante[i] > 0 && tempo_chegada[i] <= tempo_atual) {
                int tempo_a_executar = (tempo_restante[i] > quantum) ? quantum : tempo_restante[i];
                tempo_atual += tempo_a_executar;
                tempo_restante[i] -= tempo_a_executar;
                printf("Processo %d - Executando: %d unidades de tempo. Tempo restante: %d\n", processos[i].id, tempo_a_executar, tempo_restante[i]);
                if (tempo_restante[i] == 0) {
                    processos_restantes--;
                }
            }
        }
    }

    free(tempo_restante);
    free(tempo_chegada);
}
