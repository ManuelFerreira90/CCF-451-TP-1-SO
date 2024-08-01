#include "./processo.h"

typedef struct tabelaProcessos
{
    ProcessoSimulado *lista_processos;
    int primeiroProcessoIndex;
    int ultimoProcessoIndex;

} tabelaProcessos;

void inicializarTabelaProcessos(tabelaProcessos *tabela);
void inserirTabelaProcessos(ProcessoSimulado processo, tabelaProcessos *tabela);
