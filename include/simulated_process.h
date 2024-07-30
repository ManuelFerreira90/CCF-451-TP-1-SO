#ifndef SIMULATED_PROCESS_H
#define SIMULATED_PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

void handle_N(int n);
void handle_D(int x);
void handle_V(int x, int value);
void handle_A(int x, int value);
void handle_S(int x, int value);
void handle_F();
void handle_R(char *file);
void handle_T();
int create_simulated_process(const char *commands[]);

#endif
