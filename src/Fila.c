#include "../headers/Fila.h"

void inicializarFilaDinamica(FilaDinamica *fila)
{
    fila->frente = NULL;
    fila->tras = NULL;
    fila->tamanho = 0;
}

// Verifica se a fila dinâmica está vazia
int isFilaDinamicaVazia(FilaDinamica *fila)
{
    return (fila->tamanho == 0);
}

// Adiciona um item à fila dinâmica
void enfileirarDinamica(FilaDinamica *fila, int item)
{
    Node *novoNode = (Node *)malloc(sizeof(Node));
    if (novoNode == NULL)
    {
        printf("Erro: não foi possível alocar memória!\n");
        exit(EXIT_FAILURE);
    }
    novoNode->dado = item;
    novoNode->proximo = NULL;

    if (isFilaDinamicaVazia(fila))
    {
        fila->frente = novoNode;
    }
    else
    {
        fila->tras->proximo = novoNode;
    }
    fila->tras = novoNode;
    fila->tamanho++;
}

// Remove e retorna o item na frente da fila dinâmica
int desenfileirarDinamica(FilaDinamica *fila)
{
    if (isFilaDinamicaVazia(fila))
    {
        // printf("Erro: a fila está vazia!\n");
        return -1;
    }
    Node *tempNode = fila->frente;
    int item = tempNode->dado;
    fila->frente = fila->frente->proximo;
    if (fila->frente == NULL)
    {
        fila->tras = NULL;
    }
    free(tempNode);
    fila->tamanho--;
    return item;
}

// Retorna o item na frente da fila dinâmica sem removê-lo
int frenteFilaDinamica(FilaDinamica *fila)
{
    if (isFilaDinamicaVazia(fila))
    {
        // printf("Erro: a fila está vazia!\n");
        return -1;
    }
    return fila->frente->dado;
}

// Libera toda a memória alocada pela fila dinâmica
void liberarFilaDinamica(FilaDinamica *fila)
{
    while (!isFilaDinamicaVazia(fila))
    {
        desenfileirarDinamica(fila);
    }
}

void imprimirFilaDinamica(FilaDinamica *fila)
{
    printf("Tamanho da fila: %d\n", fila->tamanho);
    if (fila->tamanho == 0)
    {
        return;
    }

    Node *tempNode = fila->frente;
    printf("Fila: ");
    while (tempNode != NULL)
    {
        printf("-> %d ", tempNode->dado);
        tempNode = tempNode->proximo;
    }
    printf("\n");
}