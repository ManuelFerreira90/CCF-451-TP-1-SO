#ifndef GERENCIADOR_PROCESSOS_H
#define GERENCIADOR_PROCESSOS_H

#include "./tabela_de_processos.h"


typedef struct {
    FilaDinamica filasProntos[NUM_PRIORIDADES];
    FilaDinamica filasBloqueados[NUM_PRIORIDADES];
} EstruturaFilaPrioridades;

typedef struct {
    FilaDinamica filaPronto;
    FilaDinamica filaBloqueado;
} EstruturaRoundRobin;

typedef union {
    EstruturaFilaPrioridades filaPrioridades;
    EstruturaRoundRobin roundRobin;
} EstruturaEscalonamento;


// typedef struct {
//     FilaDinamica filasProntos[NUM_PRIORIDADES];
//     FilaDinamica filasBloqueados[NUM_PRIORIDADES];
//     int *filasEmExecucao;
//     int processoEmExecucao;
// } EstadosProcessos;

typedef struct {
    tabelaProcessos TabelaProcessos;
    EstruturaEscalonamento EstruturaEscalonamento;
    Tempo tempoAtual;
    CPU *cpus;
    int quantidadeCPUs;
    int algoritmoEscalonamento;  // 0 para fila de prioridades, 1 para round robin
    int *processosEmExecucao;
} GerenciadorProcessos;

// typedef struct {
//     tabelaProcessos TabelaProcessos;
//     EstadosProcessos EstadosProcessos;
//     Tempo tempoAtual;
//     CPU * cpus;
//     int quantidadeCPUs;
//     int *processosEmExecucao;
// } GerenciadorProcessos;

// Declarações de funções e estruturas
//---------------------------------------------------------------------------------------
void iniciarGerenciadorProcessos(GerenciadorProcessos *gerenciador, char *arquivoEntrada, int PID_Pai, int numsCPUs, int escalonador);
void adicionarProcessoProntoFilaDePrioridade(GerenciadorProcessos *gerenciador, int processoIndex);
void adicionarProcessoBloqueadoFilaDePrioridade(GerenciadorProcessos *gerenciador, int processoIndex);
void processarComando(GerenciadorProcessos * gerenciador, Instrucao instrucao, int indexCPU);

// escalonamento
void escalonadorFilaDePrioridades(GerenciadorProcessos *gerenciador);
void colocaProcessoNaCPUFilaDePrioridades(GerenciadorProcessos *gerenciador, int cpuIndex);
void executandoProcessoCPU(GerenciadorProcessos *gerenciador);
int trocaDeContexto(GerenciadorProcessos *gerenciador, int i);
int existeProcessoEmAlgumaCPU(GerenciadorProcessos *gerenciador);
void atualizaDadosProcesso(CPU *cpu);
void imprimirInstrucoes(ProcessoSimulado *processo);
void avaliarCPUVazia(GerenciadorProcessos *gerenciador);
void iniciarRoundRobin(GerenciadorProcessos *gerenciador);
void iniciarFilaDePrioridades(GerenciadorProcessos *gerenciador);

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


void trocaDeContextoFilaDePrioridade(GerenciadorProcessos *gerenciador);
void escalonadorRoundRobin(GerenciadorProcessos *gerenciador);
void trocaDeContextoRoundRobin(GerenciadorProcessos *gerenciador);
void colocaProcessoNaCPURoundRobin(GerenciadorProcessos *gerenciador, int cpuIndex);

//---------------------------------------------------------------------------------------

#endif // GERENCIADOR_PROCESSOS_H
