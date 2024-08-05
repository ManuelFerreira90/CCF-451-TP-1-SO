#ifndef TEMPO_H
#define TEMPO_H

#define MAX_PROCESSOS 5000
#define TAMANHO_MEMORIA 100
#define TABLE_WIDTH 40
#define MAX_CMD_LEN 100
#define NUM_PRIORIDADES 4


#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>  // Para open()
#include <sys/wait.h> // Para wait()
#include <errno.h>
#include "./instrucao.h"
#include "./Fila.h"



typedef struct Tempo{
    int valor;
}Tempo;


void inicializarTempo(Tempo *tempo);
void setTempo(Tempo *tempo,int valor);
void incrementarTempo(Tempo *tempo);

#endif // TEMPO_H