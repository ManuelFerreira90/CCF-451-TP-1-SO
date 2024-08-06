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

ProcessoSimulado* getProcesso(tabelaProcessos *tabela, int indice) {
    if (indice < 0 || indice >= MAX_PROCESSOS) {
        // Se o índice for inválido, retorna NULL ou trata o erro conforme desejado
        return NULL;
    }
    return tabela->listaProcessos[indice];
}

const char* estadoToString(Estados estado) {
    switch (estado) {
        case Bloqueado: return "Bloqueado";
        case Pronto: return "Pronto";
        case Execucao: return "Execução";
        default: return "Desconhecido";
    }
}

void imprimeTabelaProcessos(tabelaProcessos *tabela) {
    printf("Tabela de Processos:\n");
    for (int i = 0; i < tabela->ultimoProcessoIndex; i++) {
        printf("Processo ID: %d, Estado: %s\n", tabela->listaProcessos[i]->ID_Processo, estadoToString(tabela->listaProcessos[i]->EstadosProcesso));
    }
}
