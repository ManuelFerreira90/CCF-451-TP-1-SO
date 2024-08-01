#ifndef TEMPO_H
#define TEMPO_H

#define MAX_PROCESSOS 5000
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>  // Para open()
#include <sys/wait.h> // Para wait()
#include <errno.h>

#define MAX_CMD_LEN 100

typedef struct Tempo{
    int valor;
}Tempo;


void inicializarTempo(Tempo *tempo);
void setTempo(Tempo *tempo,int valor);
void incrementarTempo(Tempo *tempo);

#endif // TEMPO_H