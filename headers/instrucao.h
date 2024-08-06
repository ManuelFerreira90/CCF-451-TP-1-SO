#ifndef INSTRUCAO_H
#define INSTRUCAO_H




#define MAX_PROCESSOS 5000
#define TAMANHO_MEMORIA 100
#define TABLE_WIDTH 40 
#define MAX_CMD_LEN 100 
#define NUM_PRIORIDADES 4
#define NUM_CPUs 4


#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>  // Para open()
#include <sys/wait.h> // Para wait()
#include <errno.h>



typedef struct Instrucao
{
    char comando;
    int index;
    int valor;
    char arquivo[MAX_CMD_LEN];
} Instrucao;


#endif // INSTRUCAO_H