#include "../headers/tabela_de_processos.h"

int isTabelaProcessosVazia(tabelaProcessos *tabela){
    return tabela->ultimoProcessoIndex == 0;
}

int isTabelaProcessosCheia(tabelaProcessos *tabela){
    return tabela->ultimoProcessoIndex >= MAX_PROCESSOS;
}

int getTamanhoTabelaProcessos(tabelaProcessos *tabela){
    return tabela->ultimoProcessoIndex;
}

void inicializarTabelaProcessos(tabelaProcessos *tabela){
    tabela->primeiroProcessoIndex = 0;
    tabela->ultimoProcessoIndex = tabela->primeiroProcessoIndex;
}

void inserirTabelaProcessos(ProcessoSimulado *processo, tabelaProcessos *tabela) { 
    if (!isTabelaProcessosCheia(tabela)) {
        tabela->listaProcessos[tabela->ultimoProcessoIndex] = processo;
        tabela->ultimoProcessoIndex++;
    } else {
        fprintf(stderr, "Erro: Tabela de processos cheia!\n");
    }
}

void retirarTabelaProcessos(int index, tabelaProcessos *tabela) { 
    if (!isTabelaProcessosVazia(tabela) && index >= 0 && index < tabela->ultimoProcessoIndex) {
        for (int i = index; i < tabela->ultimoProcessoIndex - 1; i++) {
            tabela->listaProcessos[i] = tabela->listaProcessos[i + 1];
        }
        tabela->ultimoProcessoIndex--;
    } else {
        fprintf(stderr, "Erro: Índice inválido ou tabela de processos vazia!\n");
    }
}

int *getIndicesEstadoTabelaProcessos(tabelaProcessos *tabela, Estados estado, int *tamanhoLista){
    int tamanhoTabela = getTamanhoTabelaProcessos(tabela);
    int *lista_indices = (int*) malloc(sizeof(int) * tamanhoTabela);
    int indice_lista = 0;
    for(int x = 0; x < tamanhoTabela; x++){
        if(tabela->listaProcessos[x]->EstadosProcesso == estado){
            lista_indices[indice_lista] = x;
            indice_lista++;
        }
    }
    *tamanhoLista = indice_lista;
    return lista_indices;
}



// Função auxiliar para converter o estado do processo em uma string
const char* estadoToString(Estados estado) {
    switch (estado) {
        case Bloqueado: return "Bloqueado";
        case Pronto: return "Pronto";
        case Execucao: return "Execução";
        default: return "Desconhecido";
    }
}

// Função para imprimir um único processo
void imprimeProcesso(ProcessoSimulado *processo) {
    if (processo != NULL) {
        printf("| %-11d | %-14d | %-10d | %-11s | %-10d |\n",
               processo->ID_Processo,
               processo->ID_Processo_Pai,
               processo->PC,
               estadoToString(processo->EstadosProcesso),
               processo->prioridade);
    }
}

// Função para imprimir a tabela de processos
void imprimeTabelaProcessos(tabelaProcessos *tabela) {
    printf("+-------------+----------------+------------+-------------+------------+\n");
    printf("| ID Processo | ID Processo Pai| PC         | Estado      | Prioridade |\n");
    printf("+-------------+----------------+------------+-------------+------------+\n");

    for (int i = tabela->primeiroProcessoIndex; i <= tabela->ultimoProcessoIndex; i++) {
        if (tabela->listaProcessos[i] != NULL) {
            imprimeProcesso(tabela->listaProcessos[i]);
        }
    }

    printf("+-------------+----------------+------------+-------------+------------+\n");
}

