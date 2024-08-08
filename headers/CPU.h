#ifndef CPU_H
#define CPU_H

#include "./processo.h"

typedef struct CPU
{
    int contadorPrograma; // Valor atual do contadorPrograma
    int *memoriaVect;     // Vetor de Memória
    Tempo fatiaTempo;
    Tempo tempoUsado;
    int quantidadeInteiros;
    ProcessoSimulado *processoEmExecucao;
} CPU;

// Declarações de funções e estruturas
//---------------------------------------------------------------------------------------

void iniciarCPU(CPU *cpu);
void imprimeCPU(CPU cpu, int index);
void printTableBorder();

//---------------------------------------------------------------------------------------

#endif