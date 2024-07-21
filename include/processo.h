#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int tempo_chegada;
    int tempo_execucao;
} Processo;

Processo* ler_dados_arquivo(const char* nome_arquivo, int* num_processos);
