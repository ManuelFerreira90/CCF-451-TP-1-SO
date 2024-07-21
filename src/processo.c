#include "../include/processo.h"

Processo* ler_dados_arquivo(const char* nome_arquivo, int* num_processos) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(EXIT_FAILURE);
    }

    fscanf(arquivo, "%d", num_processos);
    Processo* processos = (Processo*)malloc(*num_processos * sizeof(Processo));

    for (int i = 0; i < *num_processos; i++) {
        fscanf(arquivo, "%d %d %d", &processos[i].id, &processos[i].tempo_chegada, &processos[i].tempo_execucao);
    }

    fclose(arquivo);
    return processos;
}