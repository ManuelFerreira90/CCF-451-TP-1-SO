#include  "./processo.h"


typedef struct tabelaProcessos{
    Processo * lista_processos;
    int primeiroProcessoIndex;
    int ultimoProcessoIndex;
    
}tabelaProcessos;

void inicializarTabelaProcessos(tabelaProcessos * tabela);
void inserirTabelaProcessos(Processo processo, tabelaProcessos *tabela);

