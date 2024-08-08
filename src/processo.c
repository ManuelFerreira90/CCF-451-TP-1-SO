/*
Estado Bloqueado = 0
Estado Pronto = 1
Estado Execução = 2
*/
#include "../headers/processo.h"

ProcessoSimulado *inicializaProcesso(char *conjuntoInstrucoes, int quantidadeInstrucao, int PID_Pai, int ID, int algoritmoEscalonamento)
{
    ProcessoSimulado *processo;

    processo = (ProcessoSimulado *)malloc(sizeof(ProcessoSimulado));
    processo->PC = 0;
    processo->EstadosProcesso = Pronto;
    
    if (algoritmoEscalonamento == 0)
    {
        processo->prioridade = 0;
    }
    else
    {
        processo->prioridade = -1;
    }

    processo->memoria = NULL;
    processo->ID_Processo = ID;
    processo->ID_Processo_Pai = PID_Pai;
    processo->tempoInicio.valor = 0;
    processo->tempoCPU.valor = 0;
    processo->quantidadeInteiros = 0;

    processo->quantidadeInstrucao = quantidadeInstrucao;
    processo->conjuntoInstrucoes = conjuntoInstrucoes;

    return processo;
}

ProcessoSimulado *criarNovoProcessoAPartirdoPai(ProcessoSimulado *processoPai, int ID_processo)
{
    ProcessoSimulado *novoProcesso = (ProcessoSimulado *)malloc(sizeof(ProcessoSimulado));
    if (!novoProcesso)
    {
        // fprintf(stderr, "Erro ao alocar memória para novo processo.\n");
    }

    novoProcesso->ID_Processo = ID_processo;
    novoProcesso->ID_Processo_Pai = processoPai->ID_Processo;
    novoProcesso->PC = processoPai->PC + 1; // Ajusta o PC para a instrução imediatamente após a instrução F
    novoProcesso->EstadosProcesso = Pronto;
    novoProcesso->prioridade = processoPai->prioridade;

    novoProcesso->memoria = (int *)malloc(sizeof(int) * processoPai->quantidadeInteiros);
    if (!novoProcesso->memoria)
    {
        // fprintf(stderr, "Erro ao alocar memória para vetor de valores inteiros do novo processo.\n");
        free(novoProcesso);
    }
    memcpy(novoProcesso->memoria, processoPai->memoria, sizeof(int) * processoPai->quantidadeInteiros);
    novoProcesso->quantidadeInteiros = processoPai->quantidadeInteiros;

    novoProcesso->conjuntoInstrucoes = strdup(processoPai->conjuntoInstrucoes);
    if (!novoProcesso->conjuntoInstrucoes)
    {
        // fprintf(stderr, "Erro ao alocar memória para conjunto de instruções do novo processo.\n");
        free(novoProcesso->memoria);
        free(novoProcesso);
    }
    novoProcesso->quantidadeInstrucao = processoPai->quantidadeInstrucao;

    novoProcesso->tempoInicio = processoPai->tempoCPU;
    novoProcesso->tempoCPU.valor = 0;

    return novoProcesso;
}

void imprimirProcesso(ProcessoSimulado *processo)
{
    printf("=============================================\n");
    printf("|              PROCESSO                     |\n");
    printf("=============================================\n");
    printf("| %-20s | %-18d |\n", "ID", processo->ID_Processo);
    printf("| %-20s | %-18d |\n", "ID do processo pai", processo->ID_Processo_Pai);
    printf("| %-20s | %-18d |\n", "PC", processo->PC);
    printf("| %-20s | %-18d |\n", "Estado", processo->EstadosProcesso);
    if (processo->prioridade != -1) // indica que não foi usado o escalonamento por prioridade
    {
        printf("| %-20s | %-18d |\n", "Prioridade", processo->prioridade);
    }
    printf("| %-21s | %-18d |\n", "Tempo de início", processo->tempoInicio.valor);
    printf("| %-20s | %-18d |\n", "Tempo de CPU", processo->tempoCPU.valor);
    printf("| %-22s | %-18d |\n", "Qtd de instruções", processo->quantidadeInstrucao);
    printf("| %-22s | %-18s |\n", "Conj. de instruções", processo->conjuntoInstrucoes);
    printf("| %-20s | %-18d |\n", "Qtd de inteiros", processo->quantidadeInteiros);
    printf("| %-21s |", "Memória");

    for (int i = 0; i < processo->quantidadeInteiros; i++)
    {
        if (i > 0)
            printf(" ");
        printf(" %d", processo->memoria[i]);
    }

    printf("\n=============================================\n");
}