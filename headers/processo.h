#ifndef PROCESSO_H
#define PROCESSO_H

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
    int prioridade; // usado quando escalonamento por prioridade
    int *memoria;
    int quantidadeInteiros;
    char *conjuntoInstrucoes;
    Tempo tempoInicio;
    Tempo tempoCPU;
    int quantidadeInstrucao;

} ProcessoSimulado;

// Declarações de funções e estruturas
//---------------------------------------------------------------------------------------

ProcessoSimulado *inicializaProcesso(char *conjuntoInstrucoes, int quantidadeInstrucao, int PID_pai, int id, int algoritmoEscalonamento);
ProcessoSimulado *criarNovoProcessoAPartirdoPai(ProcessoSimulado *processoPai, int ID_processo);
void imprimirProcesso(ProcessoSimulado *processo);
//---------------------------------------------------------------------------------------

#endif // PROCESSO_H
