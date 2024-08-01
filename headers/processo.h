#include "./tempo.h"
typedef enum
{
    Bloqueado = 0,
    Pronto = 1,
    Execucao = 2
} Estados;

typedef struct Instrucao
{
    char funcao;
    int valor;
    int valor2;
} Instrucao;

typedef struct
{
    pid_t ID_Processo;
    pid_t ID_Processo_Pai;
    int PC;
    Estados EstadosProcesso;
    int prioridade;
    int *memoria;
    int quantidadeInteiros;
    Instrucao *instrucoes;
    int quantidadeInstrucoesExecutadas;
} ProcessoSimulado;

void inicializaProcesso(ProcessoSimulado *processo);