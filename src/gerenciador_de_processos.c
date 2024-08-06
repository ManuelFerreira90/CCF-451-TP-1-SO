#include "../headers/gerenciador_de_processos.h"

// Adiciona um processo à fila de prontos, baseado na sua prioridade
void adicionarProcessoPronto(GerenciadorProcessos *gerenciador, int processoIndex)
{
    int prioridade = gerenciador->TabelaProcessos.listaProcessos[processoIndex]->prioridade;
    enfileirarDinamica(&(gerenciador->EstadosProcessos.filasProntos[prioridade]), processoIndex);
    gerenciador->TabelaProcessos.listaProcessos[processoIndex]->EstadosProcesso = Pronto;
}

// void criarProcessoSimulado(GerenciadorProcessos *gerenciador, int n)
// {
//     // Cria e inicializa um novo processo simulado
//     ProcessoSimulado *novoProcesso = (ProcessoSimulado *)malloc(sizeof(ProcessoSimulado));
//     // Inicializar novoProcesso
//     // Adiciona à fila de processos prontos
//     adicionarProcessoPronto(gerenciador, novoProcesso);
// }

// Adiciona um processo à fila de bloqueados, baseado na sua prioridade
void adicionarProcessoBloqueado(GerenciadorProcessos *gerenciador, int processoIndex)
{
    int prioridade = gerenciador->TabelaProcessos.listaProcessos[processoIndex]->prioridade;
    enfileirarDinamica(&(gerenciador->EstadosProcessos.filasBloqueados[prioridade]), processoIndex);
    gerenciador->TabelaProcessos.listaProcessos[processoIndex]->EstadosProcesso = Bloqueado;
}

// Move o processo em execução para a fila de bloqueados
void comandoB(GerenciadorProcessos *gerenciador, int indexCPU)
{
    int processoIndex = gerenciador->EstadosProcessos.filasEmExecucao[indexCPU];
    if (processoIndex != -1)
    {
        adicionarProcessoBloqueado(gerenciador, processoIndex);
        gerenciador->EstadosProcessos.filasEmExecucao[indexCPU] = -1;
    }
    else
    {
        printf("Erro: nenhum processo em execução para ser bloqueado!\n");
    }
    iniciarCPU(&gerenciador->cpus[indexCPU]);
}

// Muda a prioridade de um processo para uma de menor prioridade
void mudarPrioridadeProcesso(GerenciadorProcessos *gerenciador, int processoIndex)
{
    ProcessoSimulado *processo = gerenciador->TabelaProcessos.listaProcessos[processoIndex];
    int prioridadeAtual = processo->prioridade;

    // Remover o processo da fila de prontos atual
    desenfileirarDinamica(&gerenciador->EstadosProcessos.filasProntos[prioridadeAtual]);

    // Aumentar a prioridade do processo se não estiver na prioridade mais baixa
    if (prioridadeAtual < NUM_PRIORIDADES - 1)
    {
        processo->prioridade++;
        printf("Mudando processo PID %d para prioridade %d\n", processo->ID_Processo, processo->prioridade);
    }
    else
    {
        printf("Processo PID %d já está na prioridade mais baixa\n", processo->ID_Processo);
    }

    // Adicionar o processo na fila de prontos da nova prioridade
    enfileirarDinamica(&gerenciador->EstadosProcessos.filasProntos[processo->prioridade], processoIndex);
}

// REAVALIAR A QUESTÃO DE TERMINAR
// Termina o processo em execução
/*
TODO:

@AlvaroGS

Lógica Burra, refazer!


*/
void terminarProcessoSimulado(GerenciadorProcessos *gerenciador)
{
    /*
    int processoIndex = gerenciador->EstadosProcessos.processoEmExecucao;
    if (processoIndex != -1) {
        gerenciador->TabelaProcessos.listaProcessos[processoIndex]->EstadosProcesso = Execucao;
        gerenciador->EstadosProcessos.processoEmExecucao = -1;
    } else {
        printf("Erro: nenhum processo em execução para ser terminado!\n");
    }
    */
}

// Cria novos processos simulados
void criarProcessoSimulado(GerenciadorProcessos *gerenciador, int n)
{
    for (int i = 0; i < n; i++)
    {
        ProcessoSimulado *novoProcesso = (ProcessoSimulado *)malloc(sizeof(ProcessoSimulado));
        // Inicialização do novo processo simulado
        novoProcesso->ID_Processo = gerenciador->TabelaProcessos.ultimoProcessoIndex + 1;
        novoProcesso->ID_Processo_Pai = 0;                   // ID do processo pai, pode ser ajustado conforme necessário
        novoProcesso->PC = 0;                                // Inicialmente, o contador de programa é 0
        novoProcesso->conjuntoInstrucoes = NULL;             // Inicializar conforme necessário
        novoProcesso->prioridade = rand() % NUM_PRIORIDADES; // Exemplo de atribuição de prioridade aleatória
        novoProcesso->EstadosProcesso = Pronto;
        novoProcesso->tempoInicio = gerenciador->tempoAtual;
        inicializarTempo(&novoProcesso->tempoCPU);

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
    cpu->quantidadeInteiros = &(valor);
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

    ProcessoSimulado *novoProcesso = criarNovoProcessoAPartirdoPai(gerenciador->cpus[indexCPU].processoEmExecucao);
    inserirTabelaProcessos(novoProcesso, &(gerenciador->TabelaProcessos));
    adicionarProcessoPronto(gerenciador, gerenciador->TabelaProcessos.ultimoProcessoIndex);
    
    novoProcesso->PC = *gerenciador->cpus[indexCPU].contadorPrograma + 1;
    gerenciador->cpus[indexCPU].contadorPrograma += valor;
    novoProcesso->PC = *gerenciador->cpus[indexCPU].contadorPrograma;

    printf("Processo %d criado a partir do processo %d\n", novoProcesso->ID_Processo, gerenciador->cpus[indexCPU].processoEmExecucao->ID_Processo);
}

void comandoR(CPU *cpu, Instrucao instrucao)
{
    cpu->processoEmExecucao->conjuntoInstrucoes = instrucao.arquivo;
    cpu->processoEmExecucao->PC = 0;
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
        break;
    case 'F':
        // Cria um novo processo simulado
        comandoF(gerenciador, indexCPU, instrucao.valor);
        printf("Comando F\n");
        break;
    case 'R':
        // Substitui o programa do processo simulado
        comandoR(&(gerenciador->cpus[indexCPU]), instrucao);
        break;
    default:
        printf("Comando desconhecido: %c\n", instrucao.comando);
        break;
    }
    gerenciador->cpus[indexCPU].fatiaTempo.valor--;
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
    inicializarTempo(&(cpu->fatiaTempo));
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
        perror("Erro ao abrir o arquivo");
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

void iniciarGerenciadorProcessos(GerenciadorProcessos *gerenciador, char *arquivoEntrada, int PID_Pai)
{
    gerenciador->cpus = (CPU *)malloc(sizeof(CPU) * NUM_CPUs);
    gerenciador->quantidadeCPUs = NUM_CPUs;

    inicializarTempo(&gerenciador->tempoAtual);
    inicializarTabelaProcessos(&(gerenciador->TabelaProcessos));

    for (int i = 0; i < NUM_PRIORIDADES; i++)
    {
        inicializarFilaDinamica(&(gerenciador->EstadosProcessos.filasProntos[i]));
        inicializarFilaDinamica(&(gerenciador->EstadosProcessos.filasBloqueados[i]));
    }

    for (int i = 0; i < NUM_CPUs; i++)
    {
        // iniciarCPU(&gerenciador->cpus[i]);
        gerenciador->EstadosProcessos.filasEmExecucao[i] = -1;
        iniciarCPU(&gerenciador->cpus[i]);
    }

    // gerenciador->EstadosProcessos.processoEmExecucao = -1;

    ProcessoSimulado *processo = inicializaProcesso(arquivoEntrada, contarQuantidadeInstrucoes(arquivoEntrada), PID_Pai, 0);
    printf("ID Processo Criado: %d", processo->ID_Processo);
    inserirTabelaProcessos(processo, &(gerenciador->TabelaProcessos));
    adicionarProcessoPronto(gerenciador, 0);
}

void printTableBorder()
{
    for (int i = 0; i < TABLE_WIDTH; i++)
    {
        printf("-");
    }
    printf("\n");
}

void imprimeCPU(CPU cpu, int index) {
    printf("\n");
    printTableBorder();
    printf("| %-32s %-3d |\n", "CPU Status", index); // Adicionado o índice aqui
    printTableBorder();
    printf("| %-25s | %8d |\n", "Contador de programa", *cpu.contadorPrograma);
    printTableBorder();
    printf("| %-25s | %8d |\n", "Fatia de tempo", cpu.fatiaTempo.valor);
    printTableBorder();
    printf("| %-36s |\n", "Memoria atual na CPU");
    printTableBorder();

    if (cpu.memoriaVect != NULL) {
        printf("| %-15s | %-18s |\n", "Index", "Valor");
        printTableBorder();
        for (int i = 0; i < *cpu.quantidadeInteiros; i++) {
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
Instrucao processarLinhaEspecifica(const char *caminhoArquivo, int numeroLinha)
{
    Instrucao instrucao;

    FILE *file = fopen(caminhoArquivo, "r");
    if (!file)
    {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", caminhoArquivo);
        exit(EXIT_FAILURE);
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
                    // *valor = v1;
                    instrucao.valor = v1;
                    // printf("Comando: %c, Valor: %d\n", cmd, *valor);
                }
                break;
            case 'A':
            case 'S':
            case 'V':
                if (sscanf(linha + 2, "%d %d", &v1, &v2) == 2)
                {
                    instrucao.index = v1;
                    instrucao.valor = v2;
                    // printf("Comando: %c, Index: %d, Valor: %d\n", cmd, *index, *valor);
                }
                break;
            case 'R':
            {
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

    if (linhaAtual < numeroLinha)
    {
        fprintf(stderr, "Erro: O arquivo %s possui menos de %d linhas.\n", caminhoArquivo, numeroLinha);
    }

    fclose(file);

    return instrucao;
}

int desenfileirarProcessoParaCPU(FilaDinamica *filaProntos)
{
    // if (!isFilaDinamicaVazia(filaProntos))
    // {

    //     return desenfileirarDinamica(filaProntos);
    // }

    // return -1; // Retorna -1 se não houver processo para desenfileirar
}
void atualizarProcessoEmExecucao(GerenciadorProcessos *gerenciador, int cpuIndex, int processoId)
{
    ProcessoSimulado *processo = getProcesso(&gerenciador->TabelaProcessos, processoId);
    
    // Atualiza a CPU com o processo em execução
    gerenciador->cpus[cpuIndex].processoEmExecucao = processo;
    
    // Atualiza os ponteiros da CPU para apontar para as variáveis do processo
    gerenciador->cpus[cpuIndex].contadorPrograma = &(processo->PC);
    gerenciador->cpus[cpuIndex].memoriaVect = processo->memoria;
    gerenciador->cpus[cpuIndex].quantidadeInteiros = &(processo->quantidadeInteiros);
    
    // Atualiza as estruturas de gerenciamento de processos
    gerenciador->EstadosProcessos.filasEmExecucao[cpuIndex] = processoId;
    processo->EstadosProcesso = Execucao;
    
    printf("CPU INDEX Processo: %d\n", processo->ID_Processo);
    printf("Processo %d escolhido para a CPU %d\n", processoId, cpuIndex);
    
    //iniciarVetorMemoria(&gerenciador->cpus[cpuIndex]);
}

void executandoProcessoCPU(GerenciadorProcessos *gerenciador)
{
    Instrucao instrucao;
    for (int i = 0; i < gerenciador->quantidadeCPUs; i++)
    {
        if (gerenciador->cpus[i].processoEmExecucao != NULL)
        {
            instrucao = processarLinhaEspecifica(gerenciador->cpus[i].processoEmExecucao->conjuntoInstrucoes, *gerenciador->cpus[i].contadorPrograma + 1);
            processarComando(gerenciador, instrucao, i);
            printf("%c,%d\n", instrucao.comando, instrucao.valor);
        }
    }
}

void colocaProcessoNaCPU(GerenciadorProcessos *gerenciador, int cpuIndex)
{
    int processoId = -1;

    // Verifica se o índice da CPU é válido
    if (cpuIndex < 0 || cpuIndex >= NUM_CPUs)
    {
        printf("Erro: índice da CPU %d é inválido!\n", cpuIndex);
        return;
    }

    // Tentativa de desenfileirar um processo para a CPU especificada
    for (int i = 0; i < NUM_PRIORIDADES; i++)
    {
        processoId = desenfileirarDinamica(&gerenciador->EstadosProcessos.filasProntos[i]);
        if (processoId != -1)
        {
            atualizarProcessoEmExecucao(gerenciador, cpuIndex, processoId);
            printf("Processo %d atribuído à CPU %d\n", processoId, cpuIndex);
            return; // Saímos da função após atribuir o processo à CPU especificada
        }
    }

    printf("Erro: não há processos prontos para execução!\n");
}

// Inicia a execução do próximo processo pronto, baseado na prioridade
// void comecaExecucao(GerenciadorProcessos *gerenciador) {
//     for(int j = 0; j < NUM_CPUs; j++){
//     for (int i = 0; i < NUM_PRIORIDADES; i++) {

//         if (!isFilaDinamicaVazia(&(gerenciador->EstadosProcessos.filasProntos[i]))) {
//             gerenciador->EstadosProcessos.filasEmExecucao[j] = desenfileirarDinamica(&(gerenciador->EstadosProcessos.filasProntos[i]));
//             gerenciador->TabelaProcessos.listaProcessos[gerenciador->EstadosProcessos.filasEmExecucao[j]]->EstadosProcesso = Execucao;
//             printf("Processo %d escolhido para a CPU",gerenciador->EstadosProcessos.filasEmExecucao[j] );

//             return;
//         }
//     }}
//     printf("Erro: não há processos prontos para execução!\n");
// }

// Executa o processo atual
void executarProcessoAtual(GerenciadorProcessos *gerenciador, int indexCPU)
{
    if (gerenciador->cpus[indexCPU].processoEmExecucao != NULL)
    {
        Instrucao instrucao = processarLinhaEspecifica(gerenciador->cpus[indexCPU].processoEmExecucao->conjuntoInstrucoes, *(gerenciador->cpus[indexCPU].contadorPrograma) + 1);
        processarComando(gerenciador, instrucao, indexCPU);
    }
    else
    {
        printf("Erro: nenhum processo em execução!\n");
    }
}

void avaliarTempoProcesso(GerenciadorProcessos *gerenciador) {
    for (int i = 0; i < gerenciador->quantidadeCPUs; i++) {
        int processoEmExecucaoID = gerenciador->EstadosProcessos.filasEmExecucao[i];
        
        if (processoEmExecucaoID != -1) { // Verifica se há um processo em execução
            ProcessoSimulado *processo = gerenciador->TabelaProcessos.listaProcessos[processoEmExecucaoID];
            
            if (gerenciador->cpus[i].fatiaTempo.valor == 0) { // Verifica a fatia de tempo do processo
                printf("Processo %d atingiu a fatia de tempo\n", processoEmExecucaoID);
                if (processo->prioridade < NUM_PRIORIDADES - 1) { // Aumenta a prioridade se não for a máxima
                    processo->prioridade++;
                }
                
                // Remove o processo da execução
                gerenciador->EstadosProcessos.filasEmExecucao[i] = -1;
                
                processo->EstadosProcesso = Pronto;
                // processo->PC = gerenciador->cpus[i].contadorPrograma;
                // processo->memoria = gerenciador->cpus[i].memoriaVect;
                // processo->tempoCPU = gerenciador->cpus[i].tempoUsado;

                iniciarCPU(&gerenciador->cpus[i]);
                
                // Move o processo para a fila de prontos da nova prioridade
                enfileirarDinamica(&gerenciador->EstadosProcessos.filasProntos[processo->prioridade], processoEmExecucaoID);
                
                printf("Processo %d movido para a fila de prontos com prioridade %d\n", processoEmExecucaoID, processo->prioridade);

                // verificar se há um processo candidato para a CPU
                colocaProcessoNaCPU(gerenciador, i);
            } 
        }
    }
}

int existeProcessoEmAlgumaCPU(GerenciadorProcessos *gerenciador)
{
    for (int cpuIndex = 0; cpuIndex < NUM_CPUs; cpuIndex++)
    {
        if (gerenciador->EstadosProcessos.filasEmExecucao[cpuIndex] != -1)
        {
            return 1; // Há um processo em execução em pelo menos uma CPU
        }
    }

    return 0; // Não há processos em execução em nenhuma CPU
}