#include "../headers/gerenciador_de_processos.h"

// Adiciona um processo à fila de prontos, baseado na sua prioridade
void adicionarProcessoPronto(GerenciadorProcessos *gerenciador, int processoIndex) {
    int prioridade = gerenciador->TabelaProcessos.listaProcessos[processoIndex]->prioridade;
    enfileirarDinamica(&(gerenciador->EstadosProcessos.filasProntos[prioridade]), processoIndex);
    gerenciador->TabelaProcessos.listaProcessos[processoIndex]->estado = PRONTO;
}


void criarProcessoSimulado(GerenciadorProcessos *gerenciador, int n)
{
    // Cria e inicializa um novo processo simulado
    ProcessoSimulado *novoProcesso = (ProcessoSimulado *)malloc(sizeof(ProcessoSimulado));
    // Inicializar novoProcesso
    // Adiciona à fila de processos prontos
    adicionarProcessoPronto(gerenciador, novoProcesso);
}

// Adiciona um processo à fila de bloqueados, baseado na sua prioridade
void adicionarProcessoBloqueado(GerenciadorProcessos *gerenciador, int processoIndex) {
    int prioridade = gerenciador->TabelaProcessos.listaProcessos[processoIndex]->prioridade;
    enfileirarDinamica(&(gerenciador->EstadosProcessos.filasBloqueados[prioridade]), processoIndex);
    gerenciador->TabelaProcessos.listaProcessos[processoIndex]->estado = BLOQUEADO;
}

// Move o processo em execução para a fila de bloqueados
void bloquearProcessoSimulado(GerenciadorProcessos *gerenciador) {
    int processoIndex = gerenciador->EstadosProcessos.processoEmExecucao;
    if (processoIndex != -1) {
        adicionarProcessoBloqueado(gerenciador, processoIndex);
        gerenciador->EstadosProcessos.processoEmExecucao = -1;
    } else {
        printf("Erro: nenhum processo em execução para ser bloqueado!\n");
    }
}

// Muda a prioridade de um processo para uma de menor prioridade
void mudarPrioridadeProcesso(GerenciadorProcessos *gerenciador, int processoIndex) {
    ProcessoSimulado *processo = gerenciador->TabelaProcessos.listaProcessos[processoIndex];
    int prioridadeAtual = processo->prioridade;
    
    // Remover o processo da fila de prontos atual
    desenfileirarDinamica(&gerenciador->EstadosProcessos.filasProntos[prioridadeAtual])

    // Aumentar a prioridade do processo se não estiver na prioridade mais baixa
    if (prioridadeAtual < NUM_PRIORIDADES - 1) {
        processo->prioridade++;
        printf("Mudando processo PID %d para prioridade %d\n", processo->PID, processo->prioridade);
    } else {
        printf("Processo PID %d já está na prioridade mais baixa\n", processo->PID);
    }

    // Adicionar o processo na fila de prontos da nova prioridade
    enfileirarDinamica(&gerenciador->EstadosProcessos.filasProntos[processo->prioridade], processoIndex)
}



// REAVALIAR A QUESTÃO DE TERMINAR
// Termina o processo em execução 
void terminarProcessoSimulado(GerenciadorProcessos *gerenciador) {
    int processoIndex = gerenciador->EstadosProcessos.processoEmExecucao;
    if (processoIndex != -1) {
        gerenciador->TabelaProcessos.listaProcessos[processoIndex]->estado = TERMINADO;
        gerenciador->EstadosProcessos.processoEmExecucao = -1;
    } else {
        printf("Erro: nenhum processo em execução para ser terminado!\n");
    }
}

// Cria novos processos simulados
void criarProcessoSimulado(GerenciadorProcessos *gerenciador, int n) {
    for (int i = 0; i < n; i++) {
        ProcessoSimulado *novoProcesso = (ProcessoSimulado *)malloc(sizeof(ProcessoSimulado));
        // Inicialização do novo processo simulado
        novoProcesso->PID = gerenciador->TabelaProcessos.ultimoProcessoIndex + 1;
        novoProcesso->PIDPai = 0; // ID do processo pai, pode ser ajustado conforme necessário
        novoProcesso->PC = 0; // Inicialmente, o contador de programa é 0
        novoProcesso->conjuntoInstrucoes = NULL; // Inicializar conforme necessário
        novoProcesso->prioridade = rand() % NUM_PRIORIDADES; // Exemplo de atribuição de prioridade aleatória
        novoProcesso->estado = PRONTO;
        novoProcesso->tempoInicio = gerenciador->tempoAtual;
        novoProcesso->tempoCPU = 0;

        // Adiciona o processo à tabela de processos
        inserirTabelaProcessos(novoProcesso, &(gerenciador->TabelaProcessos));
        // Adiciona o processo à fila de prontos
        adicionarProcessoPronto(gerenciador, gerenciador->TabelaProcessos.ultimoProcessoIndex);
    }
}

void comandoD(CPU *cpu, int index)
{
    cpu->memoriaVect[index] = 0;
    cpu->contadorPrograma++;
}

void comandoN(CPU *cpu, int valor)
{
    cpu->memoriaVect = (int *)malloc(valor * sizeof(int));
    cpu->quantidadeInteiros = valor;
    cpu->contadorPrograma++;
}

void comandoV(CPU *cpu, int index, int valor)
{
    cpu->memoriaVect[index] = valor;
    cpu->contadorPrograma++;
}

void comandoA(CPU *cpu, int index, int valor)
{
    cpu->memoriaVect[index] += valor;
    cpu->contadorPrograma++;
}

void comandoS(CPU *cpu, int index, int valor)
{
    cpu->memoriaVect[index] -= valor;
    cpu->contadorPrograma++;
}

void processarComando(GerenciadorProcessos *gerenciador, Instrucao instrucao)
{
    switch (instrucao.comando)
    {
    case 'N':
        // criar vetor de memória
        comandoN(&(gerenciador->cpu), instrucao.valor);
        break;
    case 'D':
        // Declara uma nova variável no processo atual
        comandoD(&(gerenciador->cpu), instrucao.valor); 
        break;
    case 'V':
        // Define o valor de uma variável
        comandoV(&(gerenciador->cpu), instrucao.index, instrucao.valor);
        break;
    case 'A':
        // Adiciona valor a uma variável
        comandoA(&(gerenciador->cpu), instrucao.index, instrucao.valor);
        break;
    case 'S':
        // Subtrai valor de uma variável
        comandoS(&(gerenciador->cpu), instrucao.index, instrucao.valor);
        break;
    case 'B':
        // Bloqueia o processo
        break;
    case 'T':
        // Termina o processo atual
        break;
    case 'F':
        // Cria um novo processo simulado
        break;
    case 'R':
        // Substitui o programa do processo simulado
        
        break;
    default:
        printf("Comando desconhecido: %c\n", instrucao.comando);
        break;
    }
}

void iniciarVetorMemoria(GerenciadorProcessos *gerenciador)
{
    gerenciador->cpu.memoriaVect = (int *)malloc(TAMANHO_MEMORIA * sizeof(int));
}

void iniciarCPU(GerenciadorProcessos *gerenciador)
{
    inicializarTempo(&(gerenciador->cpu.fatiaTempo));
    inicializarTempo(&(gerenciador->cpu.tempoUsado));

    gerenciador->cpu.contadorPrograma = 0;

    iniciarVetorMemoria(gerenciador);
}

void iniciarGerenciadorProcessos(GerenciadorProcessos *gerenciador, char *arquivoEntrada)
{
    inicializarTempo(&gerenciador->tempoAtual);
    inicializarTabelaProcessos(&(gerenciador->TabelaProcessos));
    
    for (int i = 0; i < NUM_PRIORIDADES; i++) {
        inicializarFilaDinamica(&(gerenciador->EstadosProcessos.filasProntos[i]));
        inicializarFilaDinamica(&(gerenciador->EstadosProcessos.filasBloqueados[i]));
    }

    gerenciador->EstadosProcessos.processoEmExecucao = -1;
    gerenciador->tempoAtual = 0;

    inicializarTabelaProcessos(&(gerenciador->TabelaProcessos));
    inserirTabelaProcessos(inicializaProcesso(arquivoEntrada), &(gerenciador->TabelaProcessos));
}

void printTableBorder() {
    for (int i = 0; i < TABLE_WIDTH; i++) {
        printf("-");
    }
    printf("\n");
}

void imprimeCPU(CPU cpu) {
    printf("\n");
    printTableBorder();
    printf("| %-36s |\n", "CPU Status");
    printTableBorder();
    printf("| %-25s | %8d |\n", "Contador de programa", cpu.contadorPrograma);
    printTableBorder();
    printf("| %-25s | %8d |\n", "Fatia de tempo", cpu.fatiaTempo.valor);
    printTableBorder();
    printf("| %-36s |\n", "Memoria atual na CPU");
    printTableBorder();

    if (cpu.memoriaVect != NULL) {
        printf("| %-15s | %-18s |\n", "Index", "Valor");
        printTableBorder();
        for (int i = 0; i < cpu.quantidadeInteiros; i++) {
            printf("|  %14d | %18d |\n", i, cpu.memoriaVect[i]);
        }
        printTableBorder();
    } else {
        printf("| %-36s |\n", "NULL");
        printTableBorder();
    }
    printf("\n");
}

// ler comandos de um arquivo
Instrucao processarLinhaEspecifica(const char *caminhoArquivo, int numeroLinha) {
    Instrucao instrucao;
    
    FILE *file = fopen(caminhoArquivo, "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", caminhoArquivo);
        exit(EXIT_FAILURE);
    }

    char linha[MAX_CMD_LEN];
    int linhaAtual = 0;
    int achou = 0;
    while (fgets(linha, sizeof(linha), file) != NULL && !achou) {
        linhaAtual++;
        if (linhaAtual == numeroLinha) {
            achou = 1;
            char cmd;
            int v1, v2;

            if (sscanf(linha, "%c", &cmd) != 1) {
                break; // Linha vazia ou inválida
            }

            instrucao.comando = cmd;

            switch (cmd) {
                case 'N':
                case 'D':
                case 'F':
                    if (sscanf(linha + 2, "%d", &v1) == 1) {
                        // *valor = v1;
                        instrucao.valor = v1;
                        // printf("Comando: %c, Valor: %d\n", cmd, *valor);
                    }
                    break;
                case 'A':
                case 'S':
                case 'V':
                    if (sscanf(linha + 2, "%d %d", &v1, &v2) == 2) {
                        instrucao.index = v1;
                        instrucao.valor = v2;
                        // printf("Comando: %c, Index: %d, Valor: %d\n", cmd, *index, *valor);
                    }
                    break;
                case 'R': {
                    // char caminhoArquivoNovo[MAX_CMD_LEN];
                    // if (sscanf(linha + 2, "%s", caminhoArquivoNovo) == 1) {
                    //     // Recursivamente processar a primeira linha do novo arquivo
                    //     processarLinhaEspecifica(valor, index, comando, caminhoArquivoNovo, 1);
                    // }
                    
                    break;
                }
                default:
                    printf("Comando desconhecido: %c\n", cmd);
                    break;
            }
            break; // Encerra a leitura após processar a linha desejada
        }
    }

    if (linhaAtual < numeroLinha) {
        fprintf(stderr, "Erro: O arquivo %s possui menos de %d linhas.\n", caminhoArquivo, numeroLinha);
    }

    fclose(file);

    return instrucao;
}

// Inicia a execução do próximo processo pronto, baseado na prioridade
void comecaExecucao(GerenciadorProcessos *gerenciador) {
    for (int i = 0; i < NUM_PRIORIDADES; i++) {
        if (!isFilaDinamicaVazia(&(gerenciador->EstadosProcessos.filasProntos[i]))) {
            gerenciador->EstadosProcessos.processoEmExecucao = desenfileirarDinamica(&(gerenciador->EstadosProcessos.filasProntos[i]));
            gerenciador->TabelaProcessos.listaProcessos[gerenciador->EstadosProcessos.processoEmExecucao]->estado = EXECUTANDO;
            return;
        }
    }
    printf("Erro: não há processos prontos para execução!\n");
}

// Executa o processo atual
void executarProcessoAtual(GerenciadorProcessos *gerenciador) {
    int processoIndex = gerenciador->EstadosProcessos.processoEmExecucao;
    if (processoIndex != -1) {
        ProcessoSimulado *processoAtual = gerenciador->TabelaProcessos.listaProcessos[processoIndex];
        Instrucao instrucao = processarLinhaEspecifica(processoAtual->conjuntoInstrucoes, processoAtual->PC + 1);
        processarComando(gerenciador, instrucao);
        processoAtual->PC++;
    } else {
        printf("Erro: nenhum processo em execução!\n");
    }
}

void avaliarTempoProcesso(GerenciadorProcessos *gerenciador, int tempoDeterminado) {
    int processoIndex = gerenciador->EstadosProcessos.processoEmExecucao;
    if (processoIndex != -1) {
        ProcessoSimulado *processoAtual = gerenciador->TabelaProcessos.listaProcessos[processoIndex];
        if ((gerenciador->tempoAtual - processoAtual->tempoInicio) >= tempoDeterminado && processoAtual->estado != TERMINADO) {
            mudarPrioridadeProcesso(gerenciador, processoIndex);
            gerenciador->EstadosProcessos.processoEmExecucao = -1;
        }
    }
}