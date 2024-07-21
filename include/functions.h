#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "processo.h"

void calcular_metricas(Processo* processos, int num_processos);
void escalonador_rr(Processo* processos, int num_processos, int quantum);
void escalonador_sjf(Processo* processos, int num_processos);
void escalonador_fcfs(Processo* processos, int num_processos);

#endif
