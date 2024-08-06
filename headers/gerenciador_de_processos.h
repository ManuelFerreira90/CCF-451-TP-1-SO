#ifndef GERENCIADOR_PROCESSOS_H
#define GERENCIADOR_PROCESSOS_H

#include "./tabela_de_processos.h"


typedef struct {
    FilaDinamica filasProntos[NUM_PRIORIDADES];
    FilaDinamica filasBloqueados[NUM_PRIORIDADES];
    int filasEmExecucao[NUM_CPUs];
    //int processoEmExecucao;
} EstadosProcessos;

typedef struct {
    tabelaProcessos TabelaProcessos;
    EstadosProcessos EstadosProcessos;
    Tempo tempoAtual;
    CPU * cpus;
    int quantidadeCPUs;
} GerenciadorProcessos;

// Declarações de funções e estruturas
//---------------------------------------------------------------------------------------
void iniciarGerenciadorProcessos(GerenciadorProcessos *gerenciador, char *arquivoEntrada, int PID);
void adicionarProcessoPronto(GerenciadorProcessos *gerenciador, int processoIndex);
void criarProcessoSimulado(GerenciadorProcessos *gerenciador, int n);
void adicionarProcessoBloqueado(GerenciadorProcessos *gerenciador, int processoIndex);
void bloquearProcessoSimulado(GerenciadorProcessos *gerenciador, int tempo);
void terminarProcessoSimulado(GerenciadorProcessos *gerenciador);
void executarProcessoAtual(GerenciadorProcessos * gerenciador, int indexCPU);
void processarComando(GerenciadorProcessos * gerenciador, Instrucao instrucao, int indexCPU);
void comecaExecucao(GerenciadorProcessos *gerenciador);
void executandoProcessoCPU(GerenciadorProcessos *gerenciador);

void imprimirInstrucoes(ProcessoSimulado *processo);

Instrucao processarLinhaEspecifica(const char *caminhoArquivo, int numeroLinha);

void iniciarVetorMemoria(CPU * cpu);
void comandoD(CPU *cpu, int index);
void comandoN(CPU *cpu, int valor);
void comandoV(CPU *cpu, int index, int valor);
void comandoA(CPU *cpu, int index, int valor);
void comandoS(CPU *cpu, int index, int valor);

void printTableBorder();

//---------------------------------------------------------------------------------------

#endif // GERENCIADOR_PROCESSOS_H
