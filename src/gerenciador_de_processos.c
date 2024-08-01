#include "../headers/tempo.h"
#include "../headers/gerenciador_de_processos.h"

void adicionarProcessoPronto(GerenciadorProcessos *gerenciador, ProcessoSimulado *processo) {
    // Adiciona o processo à fila de prontos
    // Implementar lógica para adicionar à lista de processos prontos
}


void criarProcessoSimulado(GerenciadorProcessos *gerenciador, int n) {
    // Cria e inicializa um novo processo simulado
    ProcessoSimulado *novoProcesso = (ProcessoSimulado *)malloc(sizeof(ProcessoSimulado));
    // Inicializar novoProcesso
    // Adiciona à fila de processos prontos
    adicionarProcessoPronto(gerenciador, novoProcesso);
}

void adicionarProcessoBloqueado(GerenciadorProcessos *gerenciador, ProcessoSimulado *processo, int tempo) {
    // Adiciona o processo à fila de bloqueados
    // Implementar lógica para adicionar à lista de processos bloqueados
}

void bloquearProcessoSimulado(GerenciadorProcessos *gerenciador, int tempo) {
    // Move o processo atual para a fila de bloqueados
    if (gerenciador->processoAtual) {
        adicionarProcessoBloqueado(gerenciador, gerenciador->processoAtual, tempo);
        gerenciador->processoAtual = NULL;
    }
}

void terminarProcessoSimulado(GerenciadorProcessos *gerenciador) {
    // Remove o processo atual
    if (gerenciador->processoAtual) {
        free(gerenciador->processoAtual->memoria);
        free(gerenciador->processoAtual);
        gerenciador->processoAtual = NULL;
    }
}

void executarProcessoAtual(GerenciadorProcessos *gerenciador) {
    // Executa o processo atual
    // Atualiza CPU, memória e outros recursos conforme necessário
}

void processarComando(GerenciadorProcessos *gerenciador, char comando, int argumento) {
    switch (comando) {
        case 'N':
            // Configura o número de variáveis para o novo processo
            gerenciador->cpu.quantidadeInteiros = argumento;
            break;
        case 'D':
            // Declara uma nova variável no processo atual
            // Implementar lógica para declarar uma variável
            break;
        case 'V':
            // Define o valor de uma variável
            // Implementar lógica para definir o valor
            break;
        case 'A':
            // Adiciona valor a uma variável
            // Implementar lógica para adicionar valor
            break;
        case 'S':
            // Subtrai valor de uma variável
            // Implementar lógica para subtrair valor
            break;
        case 'B':
            // Bloqueia o processo
            bloquearProcessoSimulado(gerenciador, argumento);
            break;
        case 'T':
            // Termina o processo atual
            terminarProcessoSimulado(gerenciador);
            break;
        case 'F':
            // Cria um novo processo simulado
            criarProcessoSimulado(gerenciador, argumento);
            break;
        case 'R':
            // Substitui o programa do processo simulado
            // Implementar lógica para substituir o programa
            break;
        default:
            printf("Comando desconhecido: %c\n", comando);
            break;
    }
}


void iniciarVetorMemoria(GerenciadorProcessos *gerenciador)
{
    gerenciador->cpu.memoriaVect = (int *)malloc(gerenciador->cpu.quantidadeInteiros * sizeof(int));
    for (int i = 0; i < gerenciador->cpu.quantidadeInteiros; i++)
    {
        gerenciador->cpu.memoriaVect[i] = 0;
    }
}
void iniciarCPU(GerenciadorProcessos *gerenciador)
{
    inicializarTempo(&(gerenciador->cpu.fatiaTempo));
    inicializarTempo(&(gerenciador->cpu.tempoUsado));

    gerenciador->cpu.contadorPrograma = 0;

    iniciarVetorMemoria(gerenciador);
}

void iniciarGerenciadorProcessos(GerenciadorProcessos *gerenciador)
{
    iniciarCPU((gerenciador));

    inicializarTempo(&gerenciador->tempoAtual);

    gerenciador->cpu.quantidadeInteiros = 5;
}

void imprimeCPU(CPU cpu)
{
    printf("CPU:");
    printf("\nContador de programa: %d\n", cpu.contadorPrograma);
    printf("\nFatia de tempo: %d\n", cpu.fatiaTempo.valor);
    printf("\nMemoria atual na CPU: ");

    if (cpu.memoriaVect != NULL)
    {
        printf("\n");
        for (int i = 0; i < cpu.quantidadeInteiros; i++)
        {
            printf("index: %d | valor: %d\n", i, cpu.memoriaVect[i]);
        }
    }
    else
    {
        printf("NULL\n");
    }
}