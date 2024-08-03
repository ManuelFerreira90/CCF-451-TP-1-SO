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
    int *listaExecucao;   // Lista de processos em execução
    ProcessoSimulado *processoAtual;
} GerenciadorProcessos;

// Declarações de funções e estruturas
void iniciarGerenciadorProcessos(GerenciadorProcessos *gerenciador, char *arquivoEntrada);
void iniciarCPU(GerenciadorProcessos *gerenciador);
void adicionarProcessoPronto(GerenciadorProcessos *gerenciador, ProcessoSimulado *processo);
void criarProcessoSimulado(GerenciadorProcessos *gerenciador, int n);
void adicionarProcessoBloqueado(GerenciadorProcessos *gerenciador, ProcessoSimulado *processo, int tempo);
void processarLinhaEspecifica(int *valor, int *index, const char *caminhoArquivo, int numeroLinha, char *comando);
void bloquearProcessoSimulado(GerenciadorProcessos *gerenciador, int tempo);
void terminarProcessoSimulado(GerenciadorProcessos *gerenciador);
void executarProcessoAtual(GerenciadorProcessos *gerenciador);
void processarComando(GerenciadorProcessos *gerenciador, char comando, int argumento);
void iniciarVetorMemoria(GerenciadorProcessos *gerenciador);
void imprimirInstrucoes(ProcessoSimulado *processo);

#endif // GERENCIADOR_PROCESSOS_H
