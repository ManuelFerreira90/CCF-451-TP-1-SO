#include "./tempo.h"

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
    int quantidadeInstrucoesExecutadas;
    char *conjuntoInstrucoes;
} ProcessoSimulado;

ProcessoSimulado* inicializaProcesso(char *conjuntoInstrucoes);