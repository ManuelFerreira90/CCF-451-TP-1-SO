#include "../headers/tabela_de_processos.h"



void inicializarTabelaProcessos(tabelaProcessos * tabela){
    tabela->lista_processos = (Processo *) malloc(sizeof(Processo) * MAX_PROCESSOS);
    tabela->primeiroProcessoIndex = 0;
    tabela->ultimoProcessoIndex = tabela->primeiroProcessoIndex;
}

void inserirTabelaProcessos(Processo processo, tabelaProcessos *tabela) { 
    if (tabela->ultimoProcessoIndex <= MAX_PROCESSOS) {
     
        tabela -> lista_processos[tabela -> ultimoProcessoIndex] = processo;
        tabela ->ultimoProcessoIndex++;
    }

}