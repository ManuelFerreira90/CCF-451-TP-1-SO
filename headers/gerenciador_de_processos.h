#ifndef GERENCIADOR_PROCESSOS_H
#define GERENCIADOR_PROCESSOS_H

#include "./tabela_de_processos.h"

typedef struct GerenciadorProcessos
{
    CPU cpu;
    Tempo tempoAtual;

    tabelaProcessos TabelaProcessos;
    int *listaProntos;    // Lista de processos prontos
    int *listaBloqueados; // Lista de processos bloqueados
    int tamListaProntos;
    int tamListaBloqueados;
    int Execucao;   // Lista de processos em execução
    int intrucaoAtual; // Instrução atual
    char *conjuntoInstrucoes; // Conjunto de instruções
} GerenciadorProcessos;

// Declarações de funções e estruturas
void iniciarGerenciadorProcessos(GerenciadorProcessos *gerenciador, char *arquivoEntrada, int pid);
void iniciarCPU(GerenciadorProcessos *gerenciador);
void adicionarProcessoPronto(GerenciadorProcessos *gerenciador, ProcessoSimulado *processo);
void criarProcessoSimulado(GerenciadorProcessos *gerenciador, int n);
void adicionarProcessoBloqueado(GerenciadorProcessos *gerenciador, ProcessoSimulado *processo, int tempo);
Instrucao processarLinhaEspecifica(const char *caminhoArquivo, int numeroLinha);
void bloquearProcessoSimulado(GerenciadorProcessos *gerenciador, int tempo);
void terminarProcessoSimulado(GerenciadorProcessos *gerenciador);
void executarProcessoAtual(GerenciadorProcessos *gerenciador);
void processarComando(GerenciadorProcessos *gerenciador, Instrucao instrucao);
void iniciarVetorMemoria(GerenciadorProcessos *gerenciador);
void imprimirInstrucoes(ProcessoSimulado *processo);
void comecaExecucao(GerenciadorProcessos *gerenciador);
void imprimeCPU(CPU cpu);
void comandoD(CPU *cpu, int index);
void comandoN(CPU *cpu, int valor);
void comandoV(CPU *cpu, int index, int valor);
void comandoA(CPU *cpu, int index, int valor);
void comandoS(CPU *cpu, int index, int valor);
void comandoF(GerenciadorProcessos *gerenciador, int valor);
void printTableBorder();
void atualizaTempoBloqueio(GerenciadorProcessos *gerenciador);
void trocaProcessoParaBloqueado(GerenciadorProcessos *gerenciador, int n);

#endif // GERENCIADOR_PROCESSOS_H
