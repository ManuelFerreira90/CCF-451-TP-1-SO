#include "../headers/gerenciador_de_processos.h"

// Adiciona um processo à fila de prontos, baseado na sua prioridade
void adicionarProcessoProntoFilaDePrioridade(GerenciadorProcessos *gerenciador, int processoIndex)
{
    int prioridade = gerenciador->TabelaProcessos.listaProcessos[processoIndex]->prioridade;
    enfileirarDinamica(&(gerenciador->EstruturaEscalonamento.filaPrioridades.filasProntos[prioridade]), processoIndex);
    gerenciador->TabelaProcessos.listaProcessos[processoIndex]->EstadosProcesso = Pronto;
}

// Adiciona um processo à fila de bloqueados, baseado na sua prioridade
void adicionarProcessoBloqueadoFilaDePrioridade(GerenciadorProcessos *gerenciador, int processoIndex)
{
    int prioridade = gerenciador->TabelaProcessos.listaProcessos[processoIndex]->prioridade;
    enfileirarDinamica(&(gerenciador->EstruturaEscalonamento.filaPrioridades.filasBloqueados[prioridade]), processoIndex);
    gerenciador->TabelaProcessos.listaProcessos[processoIndex]->EstadosProcesso = Bloqueado;
}

// Move o processo em execução para a fila de bloqueados
void comandoB(GerenciadorProcessos *gerenciador, int indexCPU)
{
    int processoIndex = gerenciador->processosEmExecucao[indexCPU];
    if (processoIndex != -1)
    {
        if (gerenciador->algoritmoEscalonamento == 0)
        {
            adicionarProcessoBloqueadoFilaDePrioridade(gerenciador, processoIndex);
        }
        else if (gerenciador->algoritmoEscalonamento == 1)
        {
            enfileirarDinamica(&(gerenciador->EstruturaEscalonamento.roundRobin.filaBloqueado), processoIndex);
        }
        gerenciador->processosEmExecucao[indexCPU] = -1;
    }

    iniciarCPU(&gerenciador->cpus[indexCPU]);
}

void comandoD(CPU *cpu, int index)
{
    cpu->memoriaVect[index] = 0;
    cpu->contadorPrograma++;
}

void comandoN(CPU *cpu, int valor)
{
    if (cpu->quantidadeInteiros > 0)
    {
        free(cpu->processoEmExecucao->memoria);
        cpu->processoEmExecucao->memoria = NULL;
    }

    cpu->processoEmExecucao->memoria = (int *)malloc(valor * sizeof(int));
    cpu->memoriaVect = cpu->processoEmExecucao->memoria;
    cpu->quantidadeInteiros = valor;
    cpu->processoEmExecucao->quantidadeInteiros = valor;
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

void comandoF(GerenciadorProcessos *gerenciador, int indexCPU, int valor)
{

    ProcessoSimulado *novoProcesso = criarNovoProcessoAPartirdoPai(gerenciador->cpus[indexCPU].processoEmExecucao, gerenciador->TabelaProcessos.ultimoProcessoIndex);
    inserirTabelaProcessos(novoProcesso, &(gerenciador->TabelaProcessos));

    if (gerenciador->algoritmoEscalonamento == 0)
    {
        adicionarProcessoProntoFilaDePrioridade(gerenciador, novoProcesso->ID_Processo);
    }
    else if (gerenciador->algoritmoEscalonamento == 1)
    {
        enfileirarDinamica(&(gerenciador->EstruturaEscalonamento.roundRobin.filaPronto), novoProcesso->ID_Processo);
    }

    gerenciador->cpus[indexCPU].contadorPrograma += (valor + 1);
}

void comandoR(CPU *cpu, Instrucao instrucao)
{
    strcpy(cpu->processoEmExecucao->conjuntoInstrucoes, instrucao.arquivo);
    cpu->processoEmExecucao->PC = 0;
    cpu->contadorPrograma = 0;
}

void comandoT(GerenciadorProcessos *gerenciador, int indexCPU)
{
    int processoIndex = gerenciador->processosEmExecucao[indexCPU];
    ProcessoSimulado *processo = getProcesso(&gerenciador->TabelaProcessos, processoIndex);
    if (processo != NULL)
    {
        retirarTabelaProcessos(&(gerenciador->TabelaProcessos), processoIndex);
        gerenciador->processosEmExecucao[indexCPU] = -1;
        gerenciador->tempoMedio.valor += processo->tempoCPU.valor;
        gerenciador->processosTerminados += 1;
        iniciarCPU(&gerenciador->cpus[indexCPU]);
    }
}

void processarComando(GerenciadorProcessos *gerenciador, Instrucao instrucao, int indexCPU)
{
    switch (instrucao.comando)
    {
    case 'N':
        // criar vetor de memória
        comandoN(&(gerenciador->cpus[indexCPU]), instrucao.valor);
        break;
    case 'D':
        // Declara uma nova variável no processo atual
        comandoD(&(gerenciador->cpus[indexCPU]), instrucao.valor);
        break;
    case 'V':
        // Define o valor de uma variável
        comandoV(&(gerenciador->cpus[indexCPU]), instrucao.index, instrucao.valor);
        break;
    case 'A':
        // Adiciona valor a uma variável
        comandoA(&(gerenciador->cpus[indexCPU]), instrucao.index, instrucao.valor);
        break;
    case 'S':
        // Subtrai valor de uma variável
        comandoS(&(gerenciador->cpus[indexCPU]), instrucao.index, instrucao.valor);
        break;
    case 'B':
        // Bloqueia o processo
        comandoB(gerenciador, indexCPU);
        break;
    case 'T':
        // Termina o processo atual
        comandoT(gerenciador, indexCPU);
        break;
    case 'F':
        // Cria um novo processo simulado
        comandoF(gerenciador, indexCPU, instrucao.valor);
        break;
    case 'R':
        // Substitui o programa do processo simulado
        comandoR(&(gerenciador->cpus[indexCPU]), instrucao);
        break;
    default:
        break;
    }
    gerenciador->cpus[indexCPU].fatiaTempo.valor--;
    atualizaDadosProcesso(&(gerenciador->cpus[indexCPU]));
}

void iniciarVetorMemoria(CPU *cpu)
{
    if (cpu->processoEmExecucao->memoria != NULL)
    {
        cpu->memoriaVect = cpu->processoEmExecucao->memoria;
    }
}

void iniciarCPU(CPU *cpu)
{
    cpu->processoEmExecucao = NULL;
    inicializarTempo(&(cpu->tempoUsado));

    cpu->contadorPrograma = 0;

    cpu->memoriaVect = NULL;
    setTempo(&(cpu->fatiaTempo), QUANTUM);
}

// Função que retorna o número de linhas antes do primeiro comando 'F'
int contarQuantidadeInstrucoes(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return -1; // Retorna -1 em caso de erro ao abrir o arquivo
    }

    char line[MAX_CMD_LEN];
    int line_count = 0;

    while (fgets(line, sizeof(line), file))
    {
        line_count++;
        // Verifica se a linha contém o comando 'F'
        if (strstr(line, "F") != NULL)
        {
            fclose(file);
            return line_count - 1; // Retorna o número de linhas antes do primeiro 'F'
        }
    }

    fclose(file);
    return line_count; // Retorna o total de linhas se 'F' não for encontrado
}

void iniciarGerenciadorProcessos(GerenciadorProcessos *gerenciador, char *arquivoEntrada, int PID_Pai, int numsCPUs, int escalonador)
{
    printf("Iniciando gerenciador de processos...\n");
    printf("Iniciado com %d CPUs\n", numsCPUs);

    inicializarTempo(&gerenciador->tempoAtual);
    inicializarTempo(&gerenciador->tempoMedio);

    gerenciador->processosTerminados = 0;
    gerenciador->tempoMedio.valor = 0;
    gerenciador->algoritmoEscalonamento = escalonador;

    inicializarTabelaProcessos(&(gerenciador->TabelaProcessos));
    ProcessoSimulado *processo = inicializaProcesso(arquivoEntrada, contarQuantidadeInstrucoes(arquivoEntrada), PID_Pai, 0);
    inserirTabelaProcessos(processo, &(gerenciador->TabelaProcessos));

    gerenciador->cpus = (CPU *)malloc(sizeof(CPU) * numsCPUs);
    if (gerenciador->cpus == NULL)
    {
        perror("Falha na alocação de memória para CPUs");
        exit(EXIT_FAILURE);
    }
    gerenciador->quantidadeCPUs = numsCPUs;

    // Inicializa a estrutura de escalonamento
    if (escalonador == 0)
    {
        iniciarFilaDePrioridades(gerenciador);
    }
    else if (escalonador == 1)
    {
        iniciarRoundRobin(gerenciador);
    }

    gerenciador->processosEmExecucao = (int *)malloc(sizeof(int) * gerenciador->quantidadeCPUs);
    if (gerenciador->processosEmExecucao == NULL)
    {
        perror("Falha na alocação de memória para processos em execução");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < gerenciador->quantidadeCPUs; i++)
    {
        gerenciador->processosEmExecucao[i] = -1;
        iniciarCPU(&gerenciador->cpus[i]);
    }
}

void iniciarRoundRobin(GerenciadorProcessos *gerenciador)
{
    inicializarFilaDinamica(&(gerenciador->EstruturaEscalonamento.roundRobin.filaPronto));
    inicializarFilaDinamica(&(gerenciador->EstruturaEscalonamento.roundRobin.filaBloqueado));

    enfileirarDinamica(&(gerenciador->EstruturaEscalonamento.roundRobin.filaPronto), 0);
}

void iniciarFilaDePrioridades(GerenciadorProcessos *gerenciador)
{

    for (int i = 0; i < NUM_PRIORIDADES; i++)
    {
        inicializarFilaDinamica(&(gerenciador->EstruturaEscalonamento.filaPrioridades.filasProntos[i]));
        inicializarFilaDinamica(&(gerenciador->EstruturaEscalonamento.filaPrioridades.filasBloqueados[i]));
    }

    adicionarProcessoProntoFilaDePrioridade(gerenciador, 0);
}

void imprimirTempoMedioProcessos(GerenciadorProcessos gerenciador)
{
    if (gerenciador.tempoMedio.valor)
    {
        gerenciador.tempoMedio.valor = gerenciador.tempoMedio.valor / gerenciador.processosTerminados;
    }
    printf("\n");
    printTableBorder();
    printf("Processos Terminados: %d\n", gerenciador.processosTerminados);

    printf("Tempo medio de Execução dos processos:");
    printf("%d", gerenciador.tempoMedio.valor);
    printf("\n");

    printTableBorder();
    printf("\n");
}

// ler comandos de um arquivo
Instrucao processarLinhaEspecifica(char *caminhoArquivo, int numeroLinha)
{
    Instrucao instrucao;
    instrucao.sucesso = 0;

    // printf("Processando linha %d do arquivo %s\n", numeroLinha, caminhoArquivo);

    FILE *file = fopen(caminhoArquivo, "r");
    if (!file)
    {
        // fprintf(stderr, "Erro ao abrir o arquivo %s\n", caminhoArquivo);
        return instrucao;
    }

    char linha[MAX_CMD_LEN];
    int linhaAtual = 0;
    int achou = 0;
    while (fgets(linha, sizeof(linha), file) != NULL && !achou)
    {
        linhaAtual++;
        if (linhaAtual == numeroLinha)
        {
            achou = 1;
            char cmd;
            int v1, v2;

            if (sscanf(linha, "%c", &cmd) != 1)
            {
                break; // Linha vazia ou inválida
            }

            instrucao.comando = cmd;

            switch (cmd)
            {
            case 'N':
            case 'D':
            case 'F':
                if (sscanf(linha + 2, "%d", &v1) == 1)
                {
                    instrucao.valor = v1;
                }
                break;
            case 'A':
            case 'S':
            case 'V':
                if (sscanf(linha + 2, "%d %d", &v1, &v2) == 2)
                {
                    instrucao.index = v1;
                    instrucao.valor = v2;
                }
                break;
            case 'R':
            {
                char caminhoArquivoNovo[MAX_CMD_LEN - 13]; // Reservar espaço para "./entry/" e ".txt"
                if (sscanf(linha + 2, "%s", caminhoArquivoNovo) == 1)
                {
                    snprintf(instrucao.arquivo, MAX_CMD_LEN, "./entry/%s.txt", caminhoArquivoNovo);
                }
                break;
            }
            default:
                break;
            }
            break; // Encerra a leitura após processar a linha desejada
        }
    }

    if (linhaAtual < numeroLinha)
    {
        // fprintf(stderr, "Erro: O arquivo %s possui menos de %d linhas.\n", caminhoArquivo, numeroLinha);
    }
    else
    {
        instrucao.sucesso = achou;
    }

    fclose(file);

    return instrucao;
}

void atualizarProcessoEmExecucao(GerenciadorProcessos *gerenciador, int cpuIndex, int processoId)
{
    ProcessoSimulado *processo = getProcesso(&gerenciador->TabelaProcessos, processoId);

    // Atualiza a CPU com o processo em execução
    gerenciador->cpus[cpuIndex].processoEmExecucao = processo;
    gerenciador->cpus[cpuIndex].contadorPrograma = processo->PC;

    gerenciador->cpus[cpuIndex].memoriaVect = processo->memoria;
    gerenciador->cpus[cpuIndex].quantidadeInteiros = processo->quantidadeInteiros;

    // Atualiza as estruturas de gerenciamento de processos
    gerenciador->processosEmExecucao[cpuIndex] = processoId;
    processo->EstadosProcesso = Execucao;
}

void printInstrucaoSimplificada(Instrucao instrucao, int cpuIndex, int processoId)
{
    char instrucaoStr[MAX_CMD_LEN + 10];

    switch (instrucao.comando)
    {
    case 'N':
    case 'D':
    case 'F':
    case 'T':
    case 'B':
        snprintf(instrucaoStr, sizeof(instrucaoStr), "%c %d", instrucao.comando, instrucao.valor);
        break;
    case 'A':
    case 'S':
    case 'V':
        snprintf(instrucaoStr, sizeof(instrucaoStr), "%c %d %d", instrucao.comando, instrucao.index, instrucao.valor);
        break;
    case 'R':
        snprintf(instrucaoStr, sizeof(instrucaoStr), "%c %s", instrucao.comando, instrucao.arquivo);
        break;
    default:
        snprintf(instrucaoStr, sizeof(instrucaoStr), "Comando desconhecido");
        break;
    }
    printf("CPU %d | executando processo %d | instrução (%s)\n", cpuIndex, processoId, instrucaoStr);
}

void executandoProcessoCPU(GerenciadorProcessos *gerenciador)
{
    Instrucao instrucao;
    for (int i = 0; i < gerenciador->quantidadeCPUs; i++)
    {
        if (gerenciador->cpus[i].processoEmExecucao != NULL)
        {
            instrucao = processarLinhaEspecifica(gerenciador->cpus[i].processoEmExecucao->conjuntoInstrucoes, gerenciador->cpus[i].contadorPrograma + 1);

            if (instrucao.sucesso)
            {
                printInstrucaoSimplificada(instrucao, i, gerenciador->cpus[i].processoEmExecucao->ID_Processo);
                processarComando(gerenciador, instrucao, i);
            }
        }
    }
}

void colocaProcessoNaCPUFilaDePrioridades(GerenciadorProcessos *gerenciador, int cpuIndex)
{
    int processoId = -1;

    // Verifica se o índice da CPU é válido
    if (cpuIndex < 0 || cpuIndex >= gerenciador->quantidadeCPUs)
    {
        return;
    }

    // Tentativa de desenfileirar um processo para a CPU especificada
    for (int i = 0; i < NUM_PRIORIDADES; i++)
    {
        processoId = desenfileirarDinamica(&gerenciador->EstruturaEscalonamento.filaPrioridades.filasProntos[i]);
        if (processoId != -1)
        {
            atualizarProcessoEmExecucao(gerenciador, cpuIndex, processoId);
            return; // Saímos da função após atribuir o processo à CPU especificada
        }
    }
}

int trocaDeContexto(GerenciadorProcessos *gerenciador, int i)
{
    int processoEmExecucaoID = gerenciador->processosEmExecucao[i];

    if (processoEmExecucaoID != -1)
    {
        // Verifica se há um processo em execução
        ProcessoSimulado *processo = gerenciador->TabelaProcessos.listaProcessos[processoEmExecucaoID];

        if (gerenciador->cpus[i].fatiaTempo.valor == 0)
        {
            // Verifica a fatia de tempo do processo
            if (processo->prioridade < NUM_PRIORIDADES - 1)
            {
                // Aumenta a prioridade se não for a máxima
                processo->prioridade++;
            }

            // Remove o processo da execução
            gerenciador->processosEmExecucao[i] = -1;

            processo->EstadosProcesso = Pronto;

            iniciarCPU(&gerenciador->cpus[i]);
            return processoEmExecucaoID;
        }
    }

    return -1;
}

void atualizaDadosProcesso(CPU *cpu)
{
    if (cpu->processoEmExecucao != NULL)
    {
        cpu->processoEmExecucao->PC = cpu->contadorPrograma;
        cpu->processoEmExecucao->tempoCPU.valor += cpu->tempoUsado.valor;
    }
}

void incrementarTempoCPU(GerenciadorProcessos *gerenciador)
{
    for (int x = 0; x < gerenciador->quantidadeCPUs; x++)
    {
        gerenciador->cpus[x].tempoUsado.valor += 1;
    }
}

void escalonadorFilaDePrioridades(GerenciadorProcessos *gerenciador)
{

    trocaDeContextoFilaDePrioridade(gerenciador);
    executandoProcessoCPU(gerenciador);
}

void trocaDeContextoFilaDePrioridade(GerenciadorProcessos *gerenciador)
{
    int idProcesso;
    for (int i = 0; i < gerenciador->quantidadeCPUs; i++)
    {
        idProcesso = trocaDeContexto(gerenciador, i);
        if (idProcesso != -1)
        {
            ProcessoSimulado *processo = gerenciador->TabelaProcessos.listaProcessos[idProcesso];
            if (processo->prioridade < NUM_PRIORIDADES - 1)
            {
                processo->prioridade++;
            }
            enfileirarDinamica(&gerenciador->EstruturaEscalonamento.filaPrioridades.filasProntos[processo->prioridade], idProcesso);
        }

        if (gerenciador->processosEmExecucao[i] == -1)
        {
            colocaProcessoNaCPUFilaDePrioridades(gerenciador, i);
        }
    }
}

void escalonadorRoundRobin(GerenciadorProcessos *gerenciador)
{

    trocaDeContextoRoundRobin(gerenciador);
    executandoProcessoCPU(gerenciador);
}

void trocaDeContextoRoundRobin(GerenciadorProcessos *gerenciador)
{
    int idProcesso;
    for (int i = 0; i < gerenciador->quantidadeCPUs; i++)
    {
        idProcesso = trocaDeContexto(gerenciador, i);
        if (idProcesso != -1)
        {
            enfileirarDinamica(&gerenciador->EstruturaEscalonamento.roundRobin.filaPronto, idProcesso);
        }

        if (gerenciador->processosEmExecucao[i] == -1)
        {
            colocaProcessoNaCPURoundRobin(gerenciador, i);
        }
    }
}

void colocaProcessoNaCPURoundRobin(GerenciadorProcessos *gerenciador, int cpuIndex)
{
    int processoId = -1;

    // Verifica se o índice da CPU é válido
    if (cpuIndex < 0 || cpuIndex >= gerenciador->quantidadeCPUs)
    {
        return;
    }

    // Tentativa de desenfileirar um processo para a CPU especificada
    processoId = desenfileirarDinamica(&gerenciador->EstruturaEscalonamento.roundRobin.filaPronto);
    if (processoId != -1)
    {
        atualizarProcessoEmExecucao(gerenciador, cpuIndex, processoId);
    }
}

void imprimirTodosProcessos(GerenciadorProcessos *gerenciador)
{
    for (int i = 0; i < gerenciador->TabelaProcessos.ultimoProcessoIndex; i++)
    {
        imprimirProcesso(gerenciador->TabelaProcessos.listaProcessos[i]);
    }
}

void imprimirFilas(GerenciadorProcessos *gerenciador)
{
    if (gerenciador->algoritmoEscalonamento == 0)
    {
        printf("\n===========================================================\n");
        printf("|      Filas usadas no algoritmo de escalonamento          |\n");
        printf("|                 Filas De Prioridades                     |\n");
        printf("===========================================================\n");
        for (int i = 0; i < NUM_PRIORIDADES; i++)
        {
            printf("Fila de Prontos da prioridade %d: \n", i);
            imprimirFilaDinamica(&(gerenciador->EstruturaEscalonamento.filaPrioridades.filasProntos[i]));
            printf("\n");
            printf("Fila de Bloqueados da prioridade %d: \n", i);
            imprimirFilaDinamica(&(gerenciador->EstruturaEscalonamento.filaPrioridades.filasProntos[i]));
            printf("\n");
        }
    }
    else if (gerenciador->algoritmoEscalonamento == 1)
    {
        printf("\n===========================================================\n");
        printf("|      Filas usadas no algoritmo de escalonamento         |\n");
        printf("|                     Round Robin                         |\n");
        printf("===========================================================\n");
        printf("Fila de Prontos: \n");
        imprimirFilaDinamica(&(gerenciador->EstruturaEscalonamento.roundRobin.filaPronto));
        printf("\n");
        printf("Fila de Bloqueados: \n");
        imprimirFilaDinamica(&(gerenciador->EstruturaEscalonamento.roundRobin.filaBloqueado));
        printf("\n");
    }
}