#include "../include/functions.h"

void calcular_metricas(Processo* processos, int num_processos) {
    int* tempo_espera = (int*)malloc(num_processos * sizeof(int));
    int* tempo_retorno = (int*)malloc(num_processos * sizeof(int));
    int tempo_atual = 0;

    for (int i = 0; i < num_processos; i++) {
        if (tempo_atual < processos[i].tempo_chegada) {
            tempo_atual = processos[i].tempo_chegada;
        }
        tempo_espera[i] = tempo_atual - processos[i].tempo_chegada;
        tempo_atual += processos[i].tempo_execucao;
        tempo_retorno[i] = tempo_atual - processos[i].tempo_chegada;
    }

    double media_espera = 0.0, media_retorno = 0.0;
    for (int i = 0; i < num_processos; i++) {
        media_espera += tempo_espera[i];
        media_retorno += tempo_retorno[i];
    }
    media_espera /= num_processos;
    media_retorno /= num_processos;

    printf("Tempo médio de espera: %.2f\n", media_espera);
    printf("Tempo médio de retorno: %.2f\n", media_retorno);

    free(tempo_espera);
    free(tempo_retorno);
}
