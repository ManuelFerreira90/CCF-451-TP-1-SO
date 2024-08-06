#ifndef FILADINAMICA_H
#define FILADINAMICA_H

#include "./tempo.h"

typedef struct node {
    int dado;
    struct node *proximo;
} Node;

typedef struct {
    Node *frente;
    Node *tras;
    int tamanho;
} FilaDinamica;

// Funções para manipulação da fila dinâmica
// Declarações de funções e estruturas
//---------------------------------------------------------------------------------------

void inicializarFilaDinamica(FilaDinamica *fila);
int isFilaDinamicaVazia(FilaDinamica *fila);
void enfileirarDinamica(FilaDinamica *fila, int item);
int desenfileirarDinamica(FilaDinamica *fila);
int frenteFilaDinamica(FilaDinamica *fila);
void liberarFilaDinamica(FilaDinamica *fila);
void imprimirFilaDinamica(FilaDinamica *fila);
//---------------------------------------------------------------------------------------

#endif
