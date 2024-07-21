#include "../include/functions.h"

void escalonador_fcfs(Processo* processos, int num_processos) {
    int tempo_atual = 0;

    for (int i = 0; i < num_processos; i++) {
        if (tempo_atual < processos[i].tempo_chegada) {
            tempo_atual = processos[i].tempo_chegada;
        }
        tempo_atual += processos[i].tempo_execucao;
        printf("Processo %d - Início: %d, Fim: %d\n", processos[i].id, tempo_atual - processos[i].tempo_execucao, tempo_atual);
    }
}
