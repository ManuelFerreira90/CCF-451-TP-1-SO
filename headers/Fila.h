#ifndef FILADINAMICA_H
#define FILADINAMICA_H

#define NUM_PRIORIDADES 4;

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
void inicializarFilaDinamica(FilaDinamica *fila);
int isFilaDinamicaVazia(FilaDinamica *fila);
void enfileirarDinamica(FilaDinamica *fila, int item);
int desenfileirarDinamica(FilaDinamica *fila);
int frenteFilaDinamica(FilaDinamica *fila);
void liberarFilaDinamica(FilaDinamica *fila);

#endif
