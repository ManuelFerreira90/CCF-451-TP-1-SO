#ifndef GERENCIADOR_PROCESSOS_H
#define GERENCIADOR_PROCESSOS_H

#include "./tempo.h"

typedef struct CPU
{
    int contadorPrograma; // Valor atual do contadorPrograma
    int *memoriaVect;     // Vetor de Memória
    Tempo fatiaTempo;
    Tempo tempoUsado;
    int quantidadeInteiros;
} CPU;

typedef struct ProcessoSimulado
{
    int *memoria;
    int contadorPrograma;
    int quantidadeInteiros;
    // Outros campos conforme necessário
} ProcessoSimulado;

typedef struct GerenciadorProcessos
{
    CPU cpu;
    Tempo tempoAtual;
    ProcessoSimulado *filaProntos;    // Lista de processos prontos
    ProcessoSimulado *filaBloqueados; // Lista de processos bloqueados
    ProcessoSimulado *processoAtual;
} GerenciadorProcessos;

// Declarações de funções e estruturas
void iniciarGerenciadorProcessos(GerenciadorProcessos *gerenciador);
void  iniciarCPU(GerenciadorProcessos *gerenciador);

void adicionarProcessoPronto(GerenciadorProcessos *gerenciador, ProcessoSimulado *processo);
void criarProcessoSimulado(GerenciadorProcessos *gerenciador, int n);
void adicionarProcessoBloqueado(GerenciadorProcessos *gerenciador, ProcessoSimulado *processo, int tempo);

void bloquearProcessoSimulado(GerenciadorProcessos *gerenciador, int tempo);
void terminarProcessoSimulado(GerenciadorProcessos *gerenciador);
void executarProcessoAtual(GerenciadorProcessos *gerenciador);
void processarComando(GerenciadorProcessos *gerenciador, char comando, int argumento);
void iniciarVetorMemoria(GerenciadorProcessos *gerenciador);
#endif // GERENCIADOR_PROCESSOS_H
