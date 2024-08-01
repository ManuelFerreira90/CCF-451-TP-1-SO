#include "../headers/tabela_de_processos.h"

int isTabelaProcessosVazia(tabelaProcessos * tabela){
    if(tabela->ultimoProcessoIndex == 0){
        return 1;
    }
    return 0;
}
int isTabelaProcessosCheia(tabelaProcessos * tabela){
    if(tabela->ultimoProcessoIndex >= MAX_PROCESSOS){
        return 1;
    }
    return 0;
}

int getTamanhoTabelaProcessos(tabelaProcessos * tabela){
    return tabela->ultimoProcessoIndex + 1;
}

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

void retirarTabelaProcessos(int index, tabelaProcessos *tabela) { 
    if (!isTabelaProcessosVazia(tabela) && !isTabelaProcessosCheia(tabela)) { 
        
    tabela->ultimoProcessoIndex--;                 
    for (int i = index; i < tabela -> ultimoProcessoIndex; i++)
        tabela -> lista_processos[i] = tabela -> lista_processos[i + 1];

    }

} 


int * getIndicesEstadoTabelaProcessos(tabelaProcessos * tabela, Estados estado){
    int tamanhoTabela = getTamanhoTabelaProcessos(tabela);
    int * lista_indices = (int*) malloc(sizeof(int) * tamanhoTabela);
    int indice_lista = 0;
    for(int x =0;x < tamanhoTabela; x++){
        if(tabela->lista_processos[x].EstadosProcesso == estado){
            lista_indices[indice_lista] = x;
            indice_lista++;
        }
    }

}

