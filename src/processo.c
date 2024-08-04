/*
Estado Bloqueado = 0
Estado Pronto = 1
Estado Execução = 2
*/
#include "../headers/processo.h"

ProcessoSimulado* inicializaProcesso(char *conjuntoInstrucoes, int pid, int idProcesso, int pc){
    ProcessoSimulado *processo;

    processo = (ProcessoSimulado *)malloc(sizeof(ProcessoSimulado));
    processo->ID_Processo_Pai = pid;
    processo->ID_Processo = idProcesso;
    processo->PC = pc;
    processo->EstadosProcesso = Pronto;
    processo->prioridade = 0;
    processo->memoria = NULL;
    processo->conjuntoInstrucoes = conjuntoInstrucoes;
    processo->tempoBloqueio = 0;

    return processo;
}





