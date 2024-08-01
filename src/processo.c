/*
Estado Bloqueado=0
EStado Pronto = 1
Estado Execução = 2
*/
#include "../headers/processo.h"

ProcessoSimulado* inicializaProcesso(char *conjuntoInstrucoes){
    ProcessoSimulado *processo;

    // abra o arquivo de entrada e peque o segundo caracter depois do espaço
    FILE *entrada = fopen(conjuntoInstrucoes, "r");
    if (entrada == NULL)
    {
        printf("Erro ao abrir o arquivo de entrada\n");
        exit(1);
    }
    // pegue o segundo caracter depois do espaço
    char *linha = (char *)malloc(sizeof(char) * 100);
    fgets(linha, 100, entrada);
    printf("Linha: %s\n", linha);
    fclose(entrada);

    processo = (ProcessoSimulado *)malloc(sizeof(ProcessoSimulado));
    processo->PC = 0;
    processo->EstadosProcesso = Pronto;
    processo->prioridade = 0;
    processo->memoria = (int *)malloc(sizeof(int) * TAMANHO_MEMORIA);
    processo->quantidadeInteiros = 0;
    processo->quantidadeInstrucoesExecutadas = 0;
    processo->conjuntoInstrucoes = conjuntoInstrucoes;

    return processo;
}





