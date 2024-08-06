#ifndef PROCESSO_H
#define PROCESSO_H

#include "./tempo.h"
#include "./Fila.h"
typedef enum
{
    Bloqueado = 0,
    Pronto = 1,
    Execucao = 2,
} Estados;

typedef struct
{
    pid_t ID_Processo;
    pid_t ID_Processo_Pai;
    int PC;
    Estados EstadosProcesso;
    int prioridade;
    int *memoria;
    int quantidadeInteiros;
    char *conjuntoInstrucoes;
    Tempo tempoInicio;
    Tempo tempoCPU;
    int quantidadeInstrucao;


} ProcessoSimulado;

ProcessoSimulado* inicializaProcesso(char *conjuntoInstrucoes, int quantidadeInstrucao, int,int);

#endif // PROCESSO_H