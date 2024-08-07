#include "../headers/gerenciador_de_processos.h"

// Adiciona um processo à fila de prontos, baseado na sua prioridade
void adicionarProcessoPronto(GerenciadorProcessos *gerenciador, int processoIndex)
{
    int prioridade = gerenciador->TabelaProcessos.listaProcessos[processoIndex]->prioridade;
    enfileirarDinamica(&(gerenciador->EstruturaEscalonamento.filaPrioridades.filasProntos[prioridade]), processoIndex);
    gerenciador->TabelaProcessos.listaProcessos[processoIndex]->EstadosProcesso = Pronto;
}

// Adiciona um processo à fila de bloqueados, baseado na sua prioridade
void adicionarProcessoBloqueado(GerenciadorProcessos *gerenciador, int processoIndex)
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
        adicionarProcessoBloqueado(gerenciador, processoIndex);
        gerenciador->processosEmExecucao[indexCPU] = -1;
    }
    else
    {
        // printf("Erro: nenhum processo em execução para ser bloqueado!\n");
    }
    iniciarCPU(&gerenciador->cpus[indexCPU]);
}

// Muda a prioridade de um processo para uma de menor prioridade
void mudarPrioridadeProcesso(GerenciadorProcessos *gerenciador, int processoIndex)
{
    ProcessoSimulado *processo = gerenciador->TabelaProcessos.listaProcessos[processoIndex];
    int prioridadeAtual = processo->prioridade;

    // Remover o processo da fila de prontos atual
    desenfileirarDinamica(&gerenciador->EstruturaEscalonamento.filaPrioridades.filasProntos[prioridadeAtual]);

    // Aumentar a prioridade do processo se não estiver na prioridade mais baixa
    if (prioridadeAtual < NUM_PRIORIDADES - 1)
    {
        processo->prioridade++;
        // printf("Mudando processo PID %d para prioridade %d\n", processo->ID_Processo, processo->prioridade);
    }
    else
    {
        // printf("Processo PID %d já está na prioridade mais baixa\n", processo->ID_Processo);
    }

    // Adicionar o processo na fila de prontos da nova prioridade
    enfileirarDinamica(&gerenciador->EstruturaEscalonamento.filaPrioridades.filasProntos[processo->prioridade], processoIndex);
}

// REAVALIAR A QUESTÃO DE TERMINAR
// Termina o processo em execução
/*
TODO:

@AlvaroGS

Lógica Burra, refazer!


*/

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
    adicionarProcessoPronto(gerenciador, gerenciador->TabelaProcessos.ultimoProcessoIndex - 1);

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
        retirarTabelaProcessos(processoIndex, &(gerenciador->TabelaProcessos));
        gerenciador->processosEmExecucao[indexCPU] = -1;
        iniciarCPU(&gerenciador->cpus[indexCPU]);
        free(processo->memoria);
        free(processo->conjuntoInstrucoes);
        free(processo);
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
        // printf("Comando desconhecido: %c\n", instrucao.comando);
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

    // iniciarVetorMemoria(gerenciador);
    cpu->memoriaVect = NULL;
    setTempo(&(cpu->fatiaTempo), QUANTUM);
}

// Função que retorna o número de linhas antes do primeiro comando 'F'
int contarQuantidadeInstrucoes(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        // perror("Erro ao abrir o arquivo");
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
    gerenciador->cpus = (CPU *)malloc(sizeof(CPU) * numsCPUs);
    gerenciador->quantidadeCPUs = numsCPUs;

    if (escalonador == 0)
    {
        // iniciarFilaDePrioridades(&(gerenciador->EstruturaEscalonamento.filaPrioridades));
        for (int i = 0; i < NUM_PRIORIDADES; i++)
        {
            inicializarFilaDinamica(&(gerenciador->EstruturaEscalonamento.filaPrioridades.filasProntos[i]));
            inicializarFilaDinamica(&(gerenciador->EstruturaEscalonamento.filaPrioridades.filasBloqueados[i]));
        }
    }
    else if (escalonador == 1)
    {
        iniciarRoundRobin(&(gerenciador->EstruturaEscalonamento.roundRobin));
    }

    gerenciador->processosEmExecucao = (int *)malloc(sizeof(int) * gerenciador->quantidadeCPUs);
    for (int i = 0; i < gerenciador->quantidadeCPUs; i++)
    {
        gerenciador->processosEmExecucao[i] = -1;
        iniciarCPU(&gerenciador->cpus[i]);
    }

    inicializarTempo(&gerenciador->tempoAtual);
    inicializarTabelaProcessos(&(gerenciador->TabelaProcessos));
    ProcessoSimulado *processo = inicializaProcesso(arquivoEntrada, contarQuantidadeInstrucoes(arquivoEntrada), PID_Pai, 0);
    inserirTabelaProcessos(processo, &(gerenciador->TabelaProcessos));
    adicionarProcessoPronto(gerenciador, 0);
}

void iniciarRoundRobin(EstruturaRoundRobin *roundRobin)
{
    inicializarFilaDinamica(&(roundRobin->filaPronto));
    inicializarFilaDinamica(&(roundRobin->filaBloqueado));
}

void iniciarFilaDePrioridades(EstruturaFilaPrioridades *filaPrioridades)
{
    for (int i = 0; i < NUM_PRIORIDADES; i++)
    {
        inicializarFilaDinamica(&(filaPrioridades->filasProntos[i]));
        inicializarFilaDinamica(&(filaPrioridades->filasBloqueados[i]));
    }
}

void printTableBorder()
{
    for (int i = 0; i < TABLE_WIDTH; i++)
    {
        printf("=");
    }
    printf("\n");
}

void imprimeCPU(CPU cpu, int index)
{
    printf("\n");
    printTableBorder();
    printf("| %-32s %-3d |\n", "CPU Status", index); // Adicionado o índice aqui
    printTableBorder();
    printf("| %-25s | %8d |\n", "Contador de programa", cpu.contadorPrograma);
    printTableBorder();
    printf("| %-25s | %8d |\n", "Fatia de tempo", cpu.fatiaTempo.valor);
    printTableBorder();
    printf("| %-36s |\n", "Memoria atual na CPU");
    printTableBorder();

    if (cpu.memoriaVect != NULL)
    {
        printf("| %-15s | %-18s |\n", "Index", "Valor");
        printTableBorder();
        for (int i = 0; i < cpu.quantidadeInteiros; i++)
        {
            printf("|  %14d | %18d |\n", i, cpu.memoriaVect[i]);
        }
        printTableBorder();
    }
    else
    {
        printf("| %-36s |\n", "NULL");
        printTableBorder();
    }
    printf("\n\n");
}

void printInstrucao(Instrucao instrucao)
{
    printf("========================================\n");
    printf("|              INSTRUÇÃO               |\n");
    printf("========================================\n");
    printf("| Comando  | %-26c |\n", instrucao.comando);
    printf("|--------------------------------------|\n");

    switch (instrucao.comando)
    {
    case 'N':
        printf("| Valor    | %-26d |\n", instrucao.valor);
        printf("| Ação     | %-26s |\n", "Criar novo processo");
        break;
    case 'D':
        printf("| Valor    | %-26d |\n", instrucao.valor);
        printf("| Ação     | %-26s |\n", "Destruir processo");
        break;
    case 'F':
        printf("| Valor    | %-26d |\n", instrucao.valor);
        printf("| Ação     | %-26s |\n", "Finalizar processo");
        break;
    case 'A':
        printf("| Índice   | %-26d |\n", instrucao.index);
        printf("| Valor    | %-26d |\n", instrucao.valor);
        printf("| Ação     | %-26s |\n", "Adicionar valor ao índice");
        break;
    case 'S':
        printf("| Índice   | %-26d |\n", instrucao.index);
        printf("| Valor    | %-26d |\n", instrucao.valor);
        printf("| Ação     | %-26s |\n", "Subtrair valor do índice");
        break;
    case 'V':
        printf("| Índice   | %-26d |\n", instrucao.index);
        printf("| Valor    | %-26d |\n", instrucao.valor);
        printf("| Ação     | %-26s |\n", "Visualizar valor no índice");
        break;
    case 'R':
        printf("| Arquivo  | %-26s |\n", instrucao.arquivo);
        printf("| Ação     | %-26s |\n", "Ler novo arquivo de instruções");
        break;
    case 'B':
        printf("| Ação     | %-26s |\n", "Bloquear processo");
        break;
    case 'T':
        printf("| Ação     | %-26s |\n", "Terminar processo");
        break;
    default:
        printf("| Desconhecido                         |\n");
        break;
    }
    printf("========================================\n");
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

    if (instrucao.sucesso)
    {
        printInstrucao(instrucao);
    }

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

void printCPUInfo(int cpuIndex, ProcessoSimulado *processo, int contadorPrograma)
{
    printf("========================================\n");
    printf("|               CPU INFO               |\n");
    printf("========================================\n");
    printf("| CPU ID | %-26d |\n", cpuIndex);
    printf("|--------------------------------------|\n");
    printf("| Processo ID | %-24d |\n", processo->ID_Processo);
    printf("|--------------------------------------|\n");
    printf("| PC       | %-26d |\n", contadorPrograma);
    printf("========================================\n\n\n");
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
                printCPUInfo(i, gerenciador->cpus[i].processoEmExecucao, gerenciador->cpus[i].contadorPrograma + 1);
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
        // printf("Erro: índice da CPU %d é inválido!\n", cpuIndex);
        return;
    }

    // Tentativa de desenfileirar um processo para a CPU especificada
    for (int i = 0; i < NUM_PRIORIDADES; i++)
    {
        processoId = desenfileirarDinamica(&gerenciador->EstruturaEscalonamento.filaPrioridades.filasProntos[i]);
        if (processoId != -1)
        {
            atualizarProcessoEmExecucao(gerenciador, cpuIndex, processoId);
            // printf("Processo %d atribuído à CPU %d\n", processoId, cpuIndex);
            return; // Saímos da função após atribuir o processo à CPU especificada
        }
    }

    // printf("Erro: não há processos prontos para execução!\n");
}

int trocaDeContexto(GerenciadorProcessos *gerenciador, int i)
{
    int processoEmExecucaoID = gerenciador->processosEmExecucao[i];

    if (processoEmExecucaoID != -1)
    { // Verifica se há um processo em execução
        ProcessoSimulado *processo = gerenciador->TabelaProcessos.listaProcessos[processoEmExecucaoID];

        if (gerenciador->cpus[i].fatiaTempo.valor == 0)
        { // Verifica a fatia de tempo do processo
            if (processo->prioridade < NUM_PRIORIDADES - 1)
            { // Aumenta a prioridade se não for a máxima
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

int existeProcessoEmAlgumaCPU(GerenciadorProcessos *gerenciador)
{
    for (int cpuIndex = 0; cpuIndex < gerenciador->quantidadeCPUs; cpuIndex++)
    {
        if (gerenciador->processosEmExecucao[cpuIndex] != -1)
        {
            return 1; // Há um processo em execução em pelo menos uma CPU
        }
    }

    return 0; // Não há processos em execução em nenhuma CPU
}

void atualizaDadosProcesso(CPU *cpu)
{
    if (cpu->processoEmExecucao != NULL)
    {
        cpu->processoEmExecucao->PC = cpu->contadorPrograma;
        cpu->processoEmExecucao->tempoCPU.valor += cpu->tempoUsado.valor;
    }
}

void avaliarCPUVazia(GerenciadorProcessos *gerenciador)
{
    // for (int i = 0; i < gerenciador->quantidadeCPUs; i++)
    // {
    //     if (gerenciador->processosEmExecucao[i] == -1)
    //     {
    //         colocaProcessoNaCPU(gerenciador, i);
    //     }
    // }
}

void escalonadorFilaDePrioridades(GerenciadorProcessos *gerenciador)
{

    executandoProcessoCPU(gerenciador);
    trocaDeContextoFilaDePrioridade(gerenciador);
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

    executandoProcessoCPU(gerenciador);
    trocaDeContextoRoundRobin(gerenciador);
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
        // printf("Erro: índice da CPU %d é inválido!\n", cpuIndex);
        return;
    }

    // Tentativa de desenfileirar um processo para a CPU especificada
    processoId = desenfileirarDinamica(&gerenciador->EstruturaEscalonamento.roundRobin.filaPronto);
    if (processoId != -1)
    {
        atualizarProcessoEmExecucao(gerenciador, cpuIndex, processoId);
        // printf("Processo %d atribuído à CPU %d\n", processoId, cpuIndex);
    }
    else
    {
        // printf("Erro: não há processos prontos para execução!\n");
    }
}