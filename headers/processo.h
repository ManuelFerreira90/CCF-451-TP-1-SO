#include "./tempo.h"
typedef enum {Bloqueado=0, Pronto=1, Execucao=2} Estados;




typedef struct{

pid_t ID_Processo;
pid_t ID_Processo_Pai;
int PC;

Tempo TempoInicio;
Tempo TempoCPU;

Estados EstadosProcesso;
int prioridade;

}Processo;


void inicializaProcesso(Processo * processo);