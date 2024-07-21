#include "../include/functions.h"

void escalonador_sjf(Processo* processos, int num_processos) {
    // Ordenar processos por tempo de execução (e tempo de chegada em caso de empate)
    for (int i = 0; i < num_processos - 1; i++) {
        for (int j = i + 1; j < num_processos; j++) {
            if (processos[j].tempo_execucao < processos[i].tempo_execucao ||
                (processos[j].tempo_execucao == processos[i].tempo_execucao && processos[j].tempo_chegada < processos[i].tempo_chegada)) {
                Processo temp = processos[i];
                processos[i] = processos[j];
                processos[j] = temp;
            }
        }
    }

    int tempo_atual = 0;

    for (int i = 0; i < num_processos; i++) {
        if (tempo_atual < processos[i].tempo_chegada) {
            tempo_atual = processos[i].tempo_chegada;
        }
        tempo_atual += processos[i].tempo_execucao;
        printf("Processo %d - Início: %d, Fim: %d\n", processos[i].id, tempo_atual - processos[i].tempo_execucao, tempo_atual);
    }
}
