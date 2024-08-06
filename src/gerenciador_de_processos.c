#include "../headers/gerenciador_de_processos.h"

// Adiciona um processo à fila de prontos, baseado na sua prioridade
void adicionarProcessoPronto(GerenciadorProcessos *gerenciador, int processoIndex)
{
    int prioridade = gerenciador->TabelaProcessos.listaProcessos[processoIndex]->prioridade;
    enfileirarDinamica(&(gerenciador->EstadosProcessos.filasProntos[prioridade]), processoIndex);
    gerenciador->TabelaProcessos.listaProcessos[processoIndex]->EstadosProcesso = Pronto;
}

// Adiciona um processo à fila de bloqueados, baseado na sua prioridade
void adicionarProcessoBloqueado(GerenciadorProcessos *gerenciador, int processoIndex)
{
    int prioridade = gerenciador->TabelaProcessos.listaProcessos[processoIndex]->prioridade;
    enfileirarDinamica(&(gerenciador->EstadosProcessos.filasBloqueados[prioridade]), processoIndex);
    gerenciador->TabelaProcessos.listaProcessos[processoIndex]->EstadosProcesso = Bloqueado;
}

// Move o processo em execução para a fila de bloqueados
void bloquearProcessoSimulado(GerenciadorProcessos *gerenciador, int indexCPU)
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

// Termina o processo em execução
void terminarProcessoSimulado(GerenciadorProcessos *gerenciador, int indexCPU)
{
    int processoIndex = gerenciador->EstadosProcessos.filasEmExecucao[indexCPU];
    if (processoIndex != -1)
    {
        // Libera os recursos do processo
        free(gerenciador->TabelaProcessos.listaProcessos[processoIndex]->conjuntoInstrucoes);
        free(gerenciador->TabelaProcessos.listaProcessos[processoIndex]);

        // Remove o processo da CPU
        gerenciador->EstadosProcessos.filasEmExecucao[indexCPU] = -1;

        // Atualiza o índice do último processo
        gerenciador->TabelaProcessos.ultimoProcessoIndex--;

        printf("Processo PID %d terminado\n", processoIndex);
    }
    else
    {
        printf("Erro: nenhum processo em execução para ser terminado!\n");
    }
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

void processarComando(GerenciadorProcessos *gerenciador, Instrucao instrucao, int indexCPU)
{
    switch (instrucao.comando)
    {
    case 'N':
        comandoN(&(gerenciador->cpus[indexCPU]), instrucao.valor);
        break;
    case 'D':
        comandoD(&(gerenciador->cpus[indexCPU]), instrucao.valor);
        break;
    case 'V':
        comandoV(&(gerenciador->cpus[indexCPU]), instrucao.index, instrucao.valor);
        break;
    case 'A':
        comandoA(&(gerenciador->cpus[indexCPU]), instrucao.index, instrucao.valor);
        break;
    case 'S':
        comandoS(&(gerenciador->cpus[indexCPU]), instrucao.index, instrucao.valor);
        break;
    case 'B':
        bloquearProcessoSimulado(gerenciador, indexCPU);
        escalonar(gerenciador, indexCPU);
        break;
    case 'T':
        terminarProcessoSimulado(gerenciador, indexCPU);
        escalonar(gerenciador, indexCPU);
        break;
    case 'F':
        criarProcessoSimulado(gerenciador, 1);
        break;
    case 'R':
        // Substitui o programa do processo simulado
        break;
    default:
        printf("Comando desconhecido: %c\n", instrucao.comando);
        break;
    }
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
    cpu->contadorPrograma = 0;
    cpu->memoriaVect = (int *)malloc(sizeof(int) * TAMANHO_MEMORIA);
    memset(cpu->memoriaVect, 0, sizeof(int) * TAMANHO_MEMORIA);
    cpu->quantidadeInteiros = 0;
    cpu->processoEmExecucao = NULL;
    inicializarTempo(&cpu->fatiaTempo);
    inicializarTempo(&cpu->tempoUsado);
}

// Função que retorna o número de linhas antes do primeiro comando 'F'
int contar_linhas_antes_primeiro_F(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return -1; // Retorna -1 em caso de erro ao abrir o arquivo
    }

    char line[256];
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
    return line_count; // Se não encontrou 'F', retorna o total de linhas
}

void iniciarGerenciadorProcessos(GerenciadorProcessos *gerenciador, char *arquivoEntrada, int PID) {
    inicializarTabelaProcessos(&gerenciador->TabelaProcessos);
    for (int i = 0; i < NUM_PRIORIDADES; i++) {
        inicializarFilaDinamica(&gerenciador->EstadosProcessos.filasProntos[i]);
        inicializarFilaDinamica(&gerenciador->EstadosProcessos.filasBloqueados[i]);
    }
    gerenciador->quantidadeCPUs = NUM_CPUs;
    gerenciador->cpus = (CPU *)malloc(sizeof(CPU) * NUM_CPUs);
    for (int i = 0; i < NUM_CPUs; i++) {
        iniciarCPU(&gerenciador->cpus[i]);
    }
    gerenciador->tempoAtual.valor = 0;
    criarProcessoSimulado(gerenciador, PID);
}

void printTableBorder()
{
    for (int i = 0; i < TABLE_WIDTH; i++)
    {
        printf("-");
    }
    printf("\n");
}

void imprimeCPU(CPU cpu)
{
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

    gerenciador->cpus[cpuIndex].processoEmExecucao = getProcesso(&gerenciador->TabelaProcessos, processoId);
    printf("CPU INDEX Processo: %d", gerenciador->cpus[cpuIndex].processoEmExecucao->ID_Processo);
    gerenciador->EstadosProcessos.filasEmExecucao[cpuIndex] = processoId;
    gerenciador->TabelaProcessos.listaProcessos[processoId]->EstadosProcesso = Execucao;
    printf("Processo %d escolhido para a CPU %d\n", processoId, cpuIndex);
    iniciarVetorMemoria(&gerenciador->cpus[cpuIndex]);
    gerenciador->cpus[cpuIndex].contadorPrograma = gerenciador->cpus[cpuIndex].processoEmExecucao->PC;

}

void executandoProcessoCPU(GerenciadorProcessos *gerenciador)
{
    Instrucao instrucao;
    for (int i = 0; i < gerenciador->quantidadeCPUs; i++)
    {
        if (gerenciador->cpus[i].processoEmExecucao != NULL)
        {
            instrucao = processarLinhaEspecifica(gerenciador->cpus[i].processoEmExecucao->conjuntoInstrucoes, gerenciador->cpus[i].contadorPrograma+1);
            processarComando(gerenciador, instrucao, i);
            printf("%c,%d\n",instrucao.comando,instrucao.valor);
        }
        
    }
}
void comecaExecucao(GerenciadorProcessos *gerenciador)
{
    int processoId = -1;

    for (int j = 0; j < NUM_CPUs; j++)
    {

        // Tentativa de desenfileirar um processo para a CPU j
        for (int i = 0; i < NUM_PRIORIDADES; i++)
        {

            // processoId = desenfileirarProcessoParaCPU(&(gerenciador->EstadosProcessos.filasProntos[i]));
            processoId = desenfileirarDinamica(&gerenciador->EstadosProcessos.filasProntos[i]);
            printf("\nProcessID: %d\n", processoId);
            if (processoId != -1)
            {
                atualizarProcessoEmExecucao(gerenciador, j, processoId);
                //ProcessoSimulado *processo = getProcesso(gerenciador->TabelaProcessos.listaProcessos,processoId);
                printf("\nProcesso Simulado ID: %d\n", gerenciador->TabelaProcessos.listaProcessos[0]->ID_Processo);

                //gerenciador->cpus[j].processoEmExecucao
                return; // Saímos da função após atribuir o processo a uma CPU
            }
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
        Instrucao instrucao = processarLinhaEspecifica(gerenciador->cpus[indexCPU].processoEmExecucao->conjuntoInstrucoes, gerenciador->cpus[indexCPU].contadorPrograma + 1);
        processarComando(gerenciador, instrucao, indexCPU);
    }
    else
    {
        printf("Erro: nenhum processo em execução!\n");
    }
}

void avaliarTempoProcesso(GerenciadorProcessos *gerenciador, int tempoDeterminado)
{

    // int processoIndex = gerenciador->EstadosProcessos.processoEmExecucao;
    // if (processoIndex != -1) {
    //     ProcessoSimulado *processoAtual = gerenciador->TabelaProcessos.listaProcessos[processoIndex];
    //     if ((gerenciador->tempoAtual.valor - processoAtual->tempoInicio.valor) >= tempoDeterminado && processoAtual->EstadosProcesso != TERMINADO) {
    //         mudarPrioridadeProcesso(gerenciador, processoIndex);
    //         gerenciador->EstadosProcessos.processoEmExecucao = -1;
    //     }
    // }
}


int escolherProximoProcesso(GerenciadorProcessos *gerenciador)
{
    for (int prioridade = 0; prioridade < NUM_PRIORIDADES; prioridade++)
    {
        if (!isFilaDinamicaVazia(&(gerenciador->EstadosProcessos.filasProntos[prioridade])))
        {
            return desenfileirarDinamica(&(gerenciador->EstadosProcessos.filasProntos[prioridade]));
        }
    }
    return -1; // Retorna -1 se não houver processos prontos
}

void trocarContexto(GerenciadorProcessos *gerenciador, int indexCPU, int proximoProcessoIndex)
{
    int processoAtualIndex = gerenciador->EstadosProcessos.filasEmExecucao[indexCPU];

    // Se houver um processo em execução, coloca-o de volta na fila de prontos
    if (processoAtualIndex != -1)
    {
        adicionarProcessoPronto(gerenciador, processoAtualIndex);
    }

    // Atualiza a CPU com o novo processo
    gerenciador->EstadosProcessos.filasEmExecucao[indexCPU] = proximoProcessoIndex;
}


void escalonar(GerenciadorProcessos *gerenciador, int indexCPU)
{
    // Escolhe o próximo processo a ser executado
    int proximoProcessoIndex = escolherProximoProcesso(gerenciador);

    if (proximoProcessoIndex != -1)
    {
        // Realiza a troca de contexto para o próximo processo
        trocarContexto(gerenciador, indexCPU, proximoProcessoIndex);
        printf("Processo PID %d escalonado para execução na CPU %d\n", proximoProcessoIndex, indexCPU);
    }
    else
    {
        printf("Nenhum processo pronto para execução na CPU %d\n", indexCPU);
    }
}
