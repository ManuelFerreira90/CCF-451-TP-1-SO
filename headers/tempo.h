#ifndef TEMPO_H
#define TEMPO_H

#define MAX_PROCESSOS 1000
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Tempo{
    int valor;
}Tempo;


void inicializarTempo(Tempo *tempo);
void setTempo(Tempo *tempo,int valor);
void incrementarTempo(Tempo *tempo);

#endif // TEMPO_H