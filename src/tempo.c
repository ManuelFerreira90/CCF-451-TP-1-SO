#include "../headers/tempo.h"

void inicializarTempo(Tempo *tempo){
    tempo->valor = 0;
}

void setTempo(Tempo *tempo,int valor){
    tempo->valor = valor;
}

void incrementarTempo(Tempo *tempo){
    tempo->valor = tempo->valor + 1;
}
