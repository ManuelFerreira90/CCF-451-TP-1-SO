#ifndef GERENCIADOR_PROCESSOS_H
#define GERENCIADOR_PROCESSOS_H

#include "./tabela_de_processos.h"


typedef struct {
    FilaDinamica filasProntos[NUM_PRIORIDADES];
    FilaDinamica filasBloqueados[NUM_PRIORIDADES];
    int *filasEmExecucao;
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
void iniciarGerenciadorProcessos(GerenciadorProcessos *gerenciador, char *arquivoEntrada, int PID_Pai, int numsCPUs);
void adicionarProcessoPronto(GerenciadorProcessos *gerenciador, int processoIndex);
void criarProcessoSimulado(GerenciadorProcessos *gerenciador, int n);
void adicionarProcessoBloqueado(GerenciadorProcessos *gerenciador, int processoIndex);
void terminarProcessoSimulado(GerenciadorProcessos *gerenciador);
void executarProcessoAtual(GerenciadorProcessos * gerenciador, int indexCPU);
void processarComando(GerenciadorProcessos * gerenciador, Instrucao instrucao, int indexCPU);

// escalonamento
void iniciarFilaDePrioridades(GerenciadorProcessos *gerenciador);
void escalonadorFilaDePrioridades(GerenciadorProcessos *gerenciador);
void colocaProcessoNaCPU(GerenciadorProcessos *gerenciador, int indexCPU);
void executandoProcessoCPU(GerenciadorProcessos *gerenciador);
void avaliarTempoProcesso(GerenciadorProcessos *gerenciador);
int existeProcessoEmAlgumaCPU(GerenciadorProcessos *gerenciador);
void atualizaDadosProcesso(CPU *cpu);
void imprimirInstrucoes(ProcessoSimulado *processo);
void avaliarCPUVazia(GerenciadorProcessos *gerenciador);

Instrucao processarLinhaEspecifica(char *caminhoArquivo, int numeroLinha);

void iniciarVetorMemoria(CPU * cpu);

void comandoB(GerenciadorProcessos *gerenciador, int tempo);
void comandoD(CPU *cpu, int index);
void comandoN(CPU *cpu, int valor);
void comandoV(CPU *cpu, int index, int valor);
void comandoA(CPU *cpu, int index, int valor);
void comandoS(CPU *cpu, int index, int valor);
void comandoF(GerenciadorProcessos *gerenciador, int index, int valor);
void comandoR(CPU *cpu, Instrucao instrucao);
void comandoT(GerenciadorProcessos *gerenciador, int indexCPU);

void printTableBorder();

void lerArquivo(char *arquivoEntrada);
void lerTerminal(char* retorno);
void remove_char(char *str, char garbage);
void printInstrucao(Instrucao instrucao);
void printCPUInfo(int cpuIndex, ProcessoSimulado *processo, int contadorPrograma);


//---------------------------------------------------------------------------------------

#endif // GERENCIADOR_PROCESSOS_H
