#ifndef TABELA_DE_PROCESSOS_H
#define TABELA_DE_PROCESSOS_H

#include "./CPU.h"

typedef struct tabelaProcessos
{
    ProcessoSimulado *listaProcessos[MAX_PROCESSOS];
    int primeiroProcessoIndex;
    int ultimoProcessoIndex;

} tabelaProcessos;

// Funções de gerenciamento da tabela de processos
int isTabelaProcessosVazia(tabelaProcessos *tabela);
int isTabelaProcessosCheia(tabelaProcessos *tabela);
int getTamanhoTabelaProcessos(tabelaProcessos *tabela);
void inicializarTabelaProcessos(tabelaProcessos *tabela);
void inserirTabelaProcessos(ProcessoSimulado *processo, tabelaProcessos *tabela);
void retirarTabelaProcessos(int index, tabelaProcessos *tabela);
int *getIndicesEstadoTabelaProcessos(tabelaProcessos *tabela, Estados estado, int *tamanhoLista);

#endif