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
    int prioridade;
    int *memoria;
    int quantidadeInteiros;
    char *conjuntoInstrucoes;
    Tempo tempoInicio;
    Tempo tempoCPU;
    int quantidadeInstrucao;

} ProcessoSimulado;

// Declarações de funções e estruturas
//---------------------------------------------------------------------------------------

ProcessoSimulado *inicializaProcesso(char *conjuntoInstrucoes, int quantidadeInstrucao, int PID_pai, int id);
ProcessoSimulado *criarNovoProcessoAPartirdoPai(ProcessoSimulado *processoPai);
//---------------------------------------------------------------------------------------

#endif // PROCESSO_H


// void atualizarProcessoEmExecucao(GerenciadorProcessos *gerenciador, int cpuIndex, int processoId)
// {
//     ProcessoSimulado *processo = getProcesso(&gerenciador->TabelaProcessos, processoId);
    
//     // Atualiza a CPU com o processo em execução
//     gerenciador->cpus[cpuIndex].processoEmExecucao = processo;
    
//     // Atualiza os ponteiros da CPU para apontar para as variáveis do processo
//     gerenciador->cpus[cpuIndex].contadorPrograma = &(processo->PC);
//     gerenciador->cpus[cpuIndex].memoriaVect = processo->memoria;
//     gerenciador->cpus[cpuIndex].quantidadeInteiros = &(processo->quantidadeInteiros);
    
//     // Atualiza as estruturas de gerenciamento de processos
//     gerenciador->EstadosProcessos.filasEmExecucao[cpuIndex] = processoId;
//     processo->EstadosProcesso = Execucao;
    
//     printf("CPU INDEX Processo: %d\n", processo->ID_Processo);
//     printf("Processo %d escolhido para a CPU %d\n", processoId, cpuIndex);
    
//     iniciarVetorMemoria(&gerenciador->cpus[cpuIndex]);
//     gerenciador->cpus[cpuIndex].contadorPrograma = processo->PC;
// }


// typedef struct CPU
// {
//     int *contadorPrograma; // Valor atual do contadorPrograma
//     int *memoriaVect;     // Vetor de Memória
//     Tempo fatiaTempo;
//     Tempo tempoUsado;
//     int *quantidadeInteiros;
//     ProcessoSimulado *processoEmExecucao;
// } CPU;

// typedef enum
// {
//     Bloqueado = 0,
//     Pronto = 1,
//     Execucao = 2,
// } Estados;

// typedef struct
// {
//     pid_t ID_Processo;
//     pid_t ID_Processo_Pai;
//     int PC;
//     Estados EstadosProcesso;
//     int prioridade;
//     int *memoria;
//     int quantidadeInteiros;
//     char *conjuntoInstrucoes;
//     Tempo tempoInicio;
//     Tempo tempoCPU;
//     int quantidadeInstrucao;

// } ProcessoSimulado;

// quero que modifique esta função, para que os ponteiros para int da CPU recebam o endereço associado a aquela variavel do processo, para quando a CPU executar ja ficar salvo automatico qualquer alteração