/*
Estado Bloqueado = 0
Estado Pronto = 1
Estado Execução = 2
*/
#include "../headers/processo.h"

ProcessoSimulado* inicializaProcesso(char *conjuntoInstrucoes, int quantidadeInstrucao, int PID_Pai, int ID){
    ProcessoSimulado *processo;

    processo = (ProcessoSimulado *)malloc(sizeof(ProcessoSimulado));
    processo->PC = 0;
    processo->EstadosProcesso = Pronto;
    processo->prioridade = 0;
    processo->memoria = NULL;
    processo->ID_Processo = ID;
    processo->ID_Processo_Pai = PID_Pai;

    processo->quantidadeInstrucao = quantidadeInstrucao;
    processo->conjuntoInstrucoes = conjuntoInstrucoes;

    return processo;
}

ProcessoSimulado *criarNovoProcessoAPartirdoPai(ProcessoSimulado *processoPai){
    ProcessoSimulado *novoProcesso = (ProcessoSimulado *)malloc(sizeof(ProcessoSimulado));
    if (!novoProcesso) {
        fprintf(stderr, "Erro ao alocar memória para novo processo.\n");
    }

    novoProcesso->ID_Processo = processoPai->ID_Processo + 1;
    novoProcesso->ID_Processo_Pai = processoPai->ID_Processo;
    novoProcesso->PC = processoPai->PC + 1; // Ajusta o PC para a instrução imediatamente após a instrução F
    novoProcesso->EstadosProcesso = Pronto;
    novoProcesso->prioridade = processoPai->prioridade;
    
    novoProcesso->memoria = (int *)malloc(sizeof(int) * processoPai->quantidadeInteiros);
    if (!novoProcesso->memoria) {
        fprintf(stderr, "Erro ao alocar memória para vetor de valores inteiros do novo processo.\n");
        free(novoProcesso);
    }
    memcpy(novoProcesso->memoria, processoPai->memoria, sizeof(int) * processoPai->quantidadeInteiros);
    novoProcesso->quantidadeInteiros = processoPai->quantidadeInteiros;

    novoProcesso->conjuntoInstrucoes = strdup(processoPai->conjuntoInstrucoes);
    if (!novoProcesso->conjuntoInstrucoes) {
        fprintf(stderr, "Erro ao alocar memória para conjunto de instruções do novo processo.\n");
        free(novoProcesso->memoria);
        free(novoProcesso);
    }
    novoProcesso->quantidadeInstrucao = processoPai->quantidadeInstrucao;

    novoProcesso->tempoInicio = processoPai->tempoCPU;
    novoProcesso->tempoCPU.valor = 0;

    return novoProcesso;
}







