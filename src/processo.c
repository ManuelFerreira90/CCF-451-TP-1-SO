/*
Estado Bloqueado = 0
Estado Pronto = 1
Estado Execução = 2
*/
#include "../headers/processo.h"

ProcessoSimulado* inicializaProcesso(char *conjuntoInstrucoes){
    ProcessoSimulado *processo;

    processo = (ProcessoSimulado *)malloc(sizeof(ProcessoSimulado));
    processo->PC = 0;
    processo->EstadosProcesso = Pronto;
    processo->prioridade = 0;
    processo->memoria = NULL;
    processo->conjuntoInstrucoes = conjuntoInstrucoes;

    return processo;
}





