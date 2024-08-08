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
    else
    {
        // printf("Erro: nenhum processo em execução para ser bloqueado!\n");
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

    if(gerenciador->algoritmoEscalonamento == 0)
    {
        adicionarProcessoProntoFilaDePrioridade(gerenciador, novoProcesso->ID_Processo);
    }
    else if(gerenciador->algoritmoEscalonamento == 1)
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
        incrementarTempo(&(gerenciador->tempoMedio), &processo->tempoCPU);
        gerenciador->processosTerminados += 1;
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
    printf("Iniciando gerenciador de processos...\n");
    printf("Iniciado com %d CPUs\n", numsCPUs);

    inicializarTempo(&gerenciador->tempoAtual);
    inicializarTempo(&gerenciador->tempoMedio);

    gerenciador->processosTerminados = 0;
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

void printTableBorder()
{
    for (int i = 0; i < TABLE_WIDTH; i++)
    {
        printf("=");
    }
    printf("\n");
}
void imprimirTempoMedioProcessos(GerenciadorProcessos gerenciador)
{
    gerenciador.tempoMedio.valor = gerenciador.tempoMedio.valor / gerenciador.processosTerminados;
    printf("\n");
    printTableBorder();
    printf("Processos Terminados: %d\n", gerenciador.processosTerminados);

    printf("Tempo medio de Execução dos processos:");
    printf("%d", gerenciador.tempoMedio.valor);
    printf("\n");

    printTableBorder();
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
        printf("| Ação     | %-26s |\n", "Quantidade de inteiros");
        break;
    case 'D':
        printf("| Índice    | %-26d |\n", instrucao.valor);
        printf("| Ação     | %-26s |\n", "Inicializar variável");
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
        printf("| Ação     | %-26s |\n", "Variável recebe valor");
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

    // if (instrucao.sucesso)
    // {
    //     printInstrucao(instrucao);
    // }

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
                // printCPUInfo(i, gerenciador->cpus[i].processoEmExecucao, gerenciador->cpus[i].contadorPrograma + 1);
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