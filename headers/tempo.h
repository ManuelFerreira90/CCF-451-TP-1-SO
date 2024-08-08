#ifndef TEMPO_H
#define TEMPO_H

#include "./instrucao.h"

typedef struct Tempo
{
    int valor;
} Tempo;

// Declarações de funções e estruturas
//---------------------------------------------------------------------------------------
void inicializarTempo(Tempo *tempo);
void setTempo(Tempo *tempo, int valor);
void incrementarTempo(Tempo *tempo, Tempo *adicional);
//---------------------------------------------------------------------------------------

#endif // TEMPO_H