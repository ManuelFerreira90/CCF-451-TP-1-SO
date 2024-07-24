#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_PRIORIDADES 4
#define NUM_VAR 100 // Número máximo de variáveis
#define QUANTUM 4 // Quantum fixo para Round-Robin

typedef enum { PRONTO, BLOQUEADO, EXECUCAO, TERMINADO } EstadoProcesso;

typedef struct {
    int *programa;
    int contador_programa;
    int *valores_inteiros;
    int tempo_usado;
} Cpu;

typedef struct {
    int id;
    int id_pai;
    int *programa;
    int contador_programa;
    int *valores_inteiros;
    int prioridade;
    EstadoProcesso estado;
    int tempo_inicio;
    int tempo_cpu_usado;
} ProcessoSimulado;

typedef struct {
    int Tempo;
    Cpu cpu;
    ProcessoSimulado *TabelaDeProcessos;
    int *EstadoPronto[MAX_PRIORIDADES];
    int EstadoPronto_tamanhos[MAX_PRIORIDADES];
    int *EstadoBloqueado;
    int EstadoExecucao;
    int numero_nucleos;
} ProcessManager;

typedef struct {
    ProcessManager *pm;
    int id;
} Núcleo;

// Função para inicializar o gerenciador de processos
void inicializar(ProcessManager *pm, int num_processos) {
    pm->Tempo = 0;
    pm->cpu.programa = NULL;
    pm->cpu.contador_programa = 0;
    pm->cpu.valores_inteiros = malloc(NUM_VAR * sizeof(int));
    pm->cpu.tempo_usado = 0;

    pm->TabelaDeProcessos = malloc(num_processos * sizeof(ProcessoSimulado));
    memset(pm->TabelaDeProcessos, 0, num_processos * sizeof(ProcessoSimulado));

    for (int i = 0; i < MAX_PRIORIDADES; i++) {
        pm->EstadoPronto[i] = malloc(num_processos * sizeof(int));
        pm->EstadoPronto_tamanhos[i] = 0;
    }

    pm->EstadoBloqueado = malloc(num_processos * sizeof(int));
    pm->EstadoExecucao = -1;
}

// Função para adicionar um processo ao estado pronto
void adicionar_ao_estado_pronto(ProcessManager *pm, int id_processo) {
    int prioridade = pm->TabelaDeProcessos[id_processo].prioridade;
    pm->EstadoPronto[prioridade][pm->EstadoPronto_tamanhos[prioridade]++] = id_processo;
}

// Função para remover um processo do estado pronto
int remover_do_estado_pronto(ProcessManager *pm) {
    for (int i = 0; i < MAX_PRIORIDADES; i++) {
        if (pm->EstadoPronto_tamanhos[i] > 0) {
            return pm->EstadoPronto[i][0];
        }
    }
    return -1;
}

// Função para trocar contexto
void trocar_contexto(ProcessManager *pm) {
    if (pm->EstadoExecucao != -1) {
        ProcessoSimulado *processo_atual = &pm->TabelaDeProcessos[pm->EstadoExecucao];
        processo_atual->contador_programa = pm->cpu.contador_programa;
        memcpy(processo_atual->valores_inteiros, pm->cpu.valores_inteiros, sizeof(int) * NUM_VAR);

        int id_proximo_processo = remover_do_estado_pronto(pm);
        if (id_proximo_processo != -1) {
            pm->EstadoExecucao = id_proximo_processo;
            ProcessoSimulado *proximo_processo = &pm->TabelaDeProcessos[id_proximo_processo];
            pm->cpu.programa = proximo_processo->programa;
            pm->cpu.contador_programa = proximo_processo->contador_programa;
            memcpy(pm->cpu.valores_inteiros, proximo_processo->valores_inteiros, sizeof(int) * NUM_VAR);
            pm->cpu.tempo_usado = 0;
        } else {
            pm->EstadoExecucao = -1;
        }
    }
}

// Função para imprimir o estado atual
void imprimir_estado(ProcessManager *pm) {
    printf("Tempo atual: %d\n", pm->Tempo);
    printf("Estado da CPU:\n");
    printf("  Contador de programa: %d\n", pm->cpu.contador_programa);
    printf("  Tempo usado na fatia: %d\n", pm->cpu.tempo_usado);

    for (int i = 0; i < MAX_PRIORIDADES; i++) {
        printf("Fila de prioridade %d:\n", i);
        for (int j = 0; j < pm->EstadoPronto_tamanhos[i]; j++) {
            int id_processo = pm->EstadoPronto[i][j];
            ProcessoSimulado *p = &pm->TabelaDeProcessos[id_processo];
            printf("  Processo %d - Prioridade %d - Estado %d\n", p->id, p->prioridade, p->estado);
        }
    }

    printf("Estado atual do processo em execução:\n");
    if (pm->EstadoExecucao != -1) {
        ProcessoSimulado *p = &pm->TabelaDeProcessos[pm->EstadoExecucao];
        printf("  Processo %d - Contador de programa: %d\n", p->id, p->contador_programa);
    } else {
        printf("Nenhum processo está em execução.\n");
    }
}

void escalonamento(ProcessManager *pm) {
    if (pm->EstadoExecucao != -1) {
        ProcessoSimulado *processo_atual = &pm->TabelaDeProcessos[pm->EstadoExecucao];
        processo_atual->tempo_cpu_usado += pm->cpu.tempo_usado;

        if (processo_atual->tempo_cpu_usado >= QUANTUM) {
            processo_atual->prioridade = (processo_atual->prioridade + 1) % MAX_PRIORIDADES;
            adicionar_ao_estado_pronto(pm, pm->EstadoExecucao);
            pm->EstadoExecucao = -1;
        } else if (processo_atual->estado == BLOQUEADO) {
            processo_atual->prioridade = MAX_PRIORIDADES - 1;
            pm->EstadoExecucao = -1;
        }

        int id_proximo_processo = remover_do_estado_pronto(pm);
        if (id_proximo_processo != -1) {
            pm->EstadoExecucao = id_proximo_processo;
            ProcessoSimulado *proximo_processo = &pm->TabelaDeProcessos[id_proximo_processo];
            pm->cpu.programa = proximo_processo->programa;
            pm->cpu.contador_programa = proximo_processo->contador_programa;
            memcpy(pm->cpu.valores_inteiros, proximo_processo->valores_inteiros, sizeof(int) * NUM_VAR);
            pm->cpu.tempo_usado = 0;
        }
    } else {
        pm->EstadoExecucao = -1;
    }
}

void processar_instrucao(int id_processo, ProcessManager *pm) {
    ProcessoSimulado *processo = &pm->TabelaDeProcessos[id_processo];
    int instrucoes = processo->programa[processo->contador_programa];

    switch (instrucoes) {
        case 'D': {
            // Declaração de variável
            int x = processo->programa[++processo->contador_programa];
            processo->valores_inteiros[x] = 0;
            break;
        }
        case 'V': {
            // Define valor
            int x = processo->programa[++processo->contador_programa];
            int n = processo->programa[++processo->contador_programa];
            processo->valores_inteiros[x] = n;
            break;
        }
        case 'A': {
            // Adiciona valor
            int x = processo->programa[++processo->contador_programa];
            int n = processo->programa[++processo->contador_programa];
            processo->valores_inteiros[x] += n;
            break;
        }
        case 'S': {
            // Subtrai valor
            int x = processo->programa[++processo->contador_programa];
            int n = processo->programa[++processo->contador_programa];
            processo->valores_inteiros[x] -= n;
            break;
        }
        case 'B': {
            // Bloqueia o processo
            int n = processo->programa[++processo->contador_programa];
            processo->estado = BLOQUEADO;
            adicionar_ao_estado_bloqueado(pm, id_processo);
            break;
        }
        case 'T': {
            // Termina o processo
            processo->estado = TERMINADO;
            break;
        }
        case 'F': {
            // Cria um novo processo
            int n = processo->programa[++processo->contador_programa];
            ProcessoSimulado *novo_processo = &pm->TabelaDeProcessos[/*novo id*/];
            novo_processo->id_pai = id_processo;
            novo_processo->estado = PRONTO;
            adicionar_ao_estado_pronto(pm, novo_processo->id);
            break;
        }
        case 'I': {
            // Imprime o estado do sistema
            imprimir_estado(pm);
            break;
        }
        case 'M': {
            // Imprime o estado e termina o sistema
            imprimir_estado(pm);
            exit(0);
            break;
        }
    }
}

void *gerenciar_núcleo(void *arg) {
    Núcleo *núcleo = (Núcleo *)arg;
    ProcessManager *pm = núcleo->pm;

    while (1) {
        if (pm->EstadoExecucao != -1) {
            processar_instrucao(pm->EstadoExecucao, pm);
            escalonamento(pm);
        }
        // Verificar se a simulação deve ser finalizada
    }
    return NULL;
}

void iniciar_simulacao(ProcessManager *pm, int núcleos) {
    pthread_t threads[núcleos];
    Núcleo núcleos_data[núcleos];

    for (int i = 0; i < núcleos; i++) {
        núcleos_data[i].pm = pm;
        núcleos_data[i].id = i;
        pthread_create(&threads[i], NULL, gerenciar_núcleo, &núcleos_data[i]);
    }

    for (int i = 0; i < núcleos; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    int num_processos = 10;
    ProcessManager pm;
    inicializar(&pm, num_processos);

    // Configurar processos iniciais e filas

    // Iniciar simulação com um núcleo
    iniciar_simulacao(&pm, 1);

    // Comparar resultados com múltiplos núcleos
    iniciar_simulacao(&pm, 2);

    return 0;
}
