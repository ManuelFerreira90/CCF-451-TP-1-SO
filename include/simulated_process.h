#ifndef SIMULATED_PROCESS_H
#define SIMULATED_PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

void handle_N(int process_id, int n);
void handle_D(int process_id, int x);
void handle_V(int process_id, int x, int value);
void handle_A(int process_id, int x, int value);
void handle_S(int process_id, int x, int value);
void handle_F(int process_id);
void handle_R(int process_id, char *file);
void handle_T(int process_id);
int create_simulated_process(int process_id, const char *commands[]);

#endif
