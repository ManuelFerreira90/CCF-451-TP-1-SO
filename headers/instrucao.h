#ifndef INSTRUCAO_H
#define INSTRUCAO_H

typedef struct Instrucao
{
    char comando;
    int index;
    int valor;
    char arquivo[MAX_CMD_LEN];
} Instrucao;


#endif // INSTRUCAO_H