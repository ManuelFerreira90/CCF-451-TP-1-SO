#include "../headers/gerenciador_de_processos.h"

void adicionarProcessoPronto(GerenciadorProcessos *gerenciador, ProcessoSimulado *processo)
{
    // Adiciona o processo à fila de prontos
    // Implementar lógica para adicionar à lista de processos prontos
}

void criarProcessoSimulado(GerenciadorProcessos *gerenciador, int n)
{
    // Cria e inicializa um novo processo simulado
    ProcessoSimulado *novoProcesso = (ProcessoSimulado *)malloc(sizeof(ProcessoSimulado));
    // Inicializar novoProcesso
    // Adiciona à fila de processos prontos
    adicionarProcessoPronto(gerenciador, novoProcesso);
}

void adicionarProcessoBloqueado(GerenciadorProcessos *gerenciador, ProcessoSimulado *processo, int tempo)
{
    // Adiciona o processo à fila de bloqueados
    // Implementar lógica para adicionar à lista de processos bloqueados
}

// void bloquearProcessoSimulado(GerenciadorProcessos *gerenciador, int tempo)
// {
//     // Move o processo atual para a fila de bloqueados
//     // if (gerenciador->processoAtual)
//     // {
//     //     adicionarProcessoBloqueado(gerenciador, gerenciador->processoAtual, tempo);
//     //     gerenciador->processoAtual = NULL;
//     // }
// }

void terminarProcessoSimulado(GerenciadorProcessos *gerenciador)
{
    // Remove o processo atual
    // if (gerenciador->processoAtual)
    // {
    //     free(gerenciador->processoAtual->memoria);
    //     free(gerenciador->processoAtual);
    //     gerenciador->processoAtual = NULL;
    // }
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

void reiniciarCPU(CPU *cpu)
{
    cpu->contadorPrograma = 0;
    cpu->quantidadeInteiros = 0;
    cpu->fatiaTempo.valor = 0;
    cpu->tempoUsado.valor = 0;
    cpu->processoEmExecucao = NULL;
    free(cpu->memoriaVect);
    cpu->memoriaVect = NULL;
}

void trocaProcessoParaBloqueado(GerenciadorProcessos *gerenciador, int n) {
    if (gerenciador->Execucao == -1) {
        printf("Não há processo em execução.\n");
        return;
    }

    int processoID = gerenciador->Execucao;

    // Encontrar o processo em execução na tabela de processos
    ProcessoSimulado *processo = NULL;
    for (int i = gerenciador->TabelaProcessos.primeiroProcessoIndex; i <= gerenciador->TabelaProcessos.ultimoProcessoIndex; i++) {
        if (gerenciador->TabelaProcessos.listaProcessos[i] != NULL && 
            gerenciador->TabelaProcessos.listaProcessos[i]->ID_Processo == processoID) {
            processo = gerenciador->TabelaProcessos.listaProcessos[i];
            break;
        }
    }

    if (processo == NULL) {
        printf("Processo em execução não encontrado na tabela de processos.\n");
        return;
    }

    // Atualizar o estado do processo para Bloqueado e definir o tempo de bloqueio
    processo->EstadosProcesso = Bloqueado;
    processo->tempoBloqueio = n;

    // Adicionar o processo à lista de bloqueados
    gerenciador->listaBloqueados[gerenciador->tamListaBloqueados++] = processoID;

    // Remover o processo da execução
    gerenciador->Execucao = -1;

    printf("Processo %d foi movido para a lista de bloqueados por %d unidades de tempo.\n", processoID, n);

    reiniciarCPU(&(gerenciador->cpu));
}

void atualizaTempoBloqueio(GerenciadorProcessos *gerenciador) {
    for (int i = 0; i < gerenciador->tamListaBloqueados; i++) {
        int processoID = gerenciador->listaBloqueados[i];
        
        // Encontrar o processo na tabela de processos
        ProcessoSimulado *processo = NULL;
        for (int j = gerenciador->TabelaProcessos.primeiroProcessoIndex; j <= gerenciador->TabelaProcessos.ultimoProcessoIndex; j++) {
            if (gerenciador->TabelaProcessos.listaProcessos[j] != NULL && 
                gerenciador->TabelaProcessos.listaProcessos[j]->ID_Processo == processoID) {
                processo = gerenciador->TabelaProcessos.listaProcessos[j];
                break;
            }
        }

        if (processo == NULL) {
            printf("Processo bloqueado não encontrado na tabela de processos.\n");
            continue;
        }

        printf("Tempo de bloqueio do processo %d: %d\n", processoID, processo->tempoBloqueio);

        // Se o tempo de bloqueio acabou, mover o processo para a lista de prontos
        if (processo->tempoBloqueio == 0) {
            printf("Processo %d foi desbloqueado.\n", processoID);
            processo->EstadosProcesso = Pronto;
            gerenciador->listaProntos[gerenciador->tamListaProntos++] = processoID;

            // Remover o processo da lista de bloqueados
            for (int k = i; k < gerenciador->tamListaBloqueados - 1; k++) {
                gerenciador->listaBloqueados[k] = gerenciador->listaBloqueados[k + 1];
            }
            gerenciador->tamListaBloqueados--;
            i--;  // Ajustar o índice para verificar a próxima posição correta na próxima iteração
        }
        
        // Diminuir o tempo de bloqueio
        processo->tempoBloqueio--;
    }
}

void comandoF(GerenciadorProcessos *gerenciador, int valor)
{
    ProcessoSimulado *novoProcesso = inicializaProcesso(gerenciador->conjuntoInstrucoes, valor, gerenciador->TabelaProcessos.ultimoProcessoIndex + 1, 0);
    inserirTabelaProcessos(novoProcesso, &(gerenciador->TabelaProcessos));
    gerenciador->listaProntos[gerenciador->tamListaProntos++] = novoProcesso->ID_Processo;
    printf("Processo %d criado.\n", novoProcesso->ID_Processo);
}

void processarComando(GerenciadorProcessos *gerenciador, Instrucao instrucao)
{
    switch (instrucao.comando)
    {
    case 'N':
        // criar vetor de memória
        if (gerenciador->Execucao == -1)
        {
            printf("Não há processo em execução.\n");
            break;
        }
        comandoN(&(gerenciador->cpu), instrucao.valor);
        break;
    case 'D':
        // Declara uma nova variável no processo atual
        if (gerenciador->Execucao == -1)
        {
            printf("Não há processo em execução.\n");
            break;
        }
        comandoD(&(gerenciador->cpu), instrucao.valor); 
        break;
    case 'V':
        // Define o valor de uma variável
        if (gerenciador->Execucao == -1)
        {
            printf("Não há processo em execução.\n");
            break;
        }
        comandoV(&(gerenciador->cpu), instrucao.index, instrucao.valor);
        break;
    case 'A':
        // Adiciona valor a uma variável
        if (gerenciador->Execucao == -1)
        {
            printf("Não há processo em execução.\n");
            break;
        }
        comandoA(&(gerenciador->cpu), instrucao.index, instrucao.valor);
        break;
    case 'S':
        // Subtrai valor de uma variável
        if (gerenciador->Execucao == -1)
        {
            printf("Não há processo em execução.\n");
            break;
        }
        comandoS(&(gerenciador->cpu), instrucao.index, instrucao.valor);
        break;
    case 'B':
        // Bloqueia o processo
        if (gerenciador->Execucao == -1)
        {
            printf("Não há processo em execução.\n");
            break;
        }
        printf("Bloqueando processo %d por %d unidades de tempo.\n", gerenciador->Execucao, instrucao.valor);
        trocaProcessoParaBloqueado(gerenciador, instrucao.valor);
        break;
    case 'T':
        // Termina o processo atual
        break;
    case 'F':
        // Cria um novo processo simulado
        comandoF(gerenciador, instrucao.valor);
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

void iniciarGerenciadorProcessos(GerenciadorProcessos *gerenciador, char *arquivoEntrada, int pid)
{
    inicializarTempo(&gerenciador->tempoAtual);
    inicializarTabelaProcessos(&(gerenciador->TabelaProcessos));

    gerenciador->listaProntos = (int *)malloc(sizeof(int) * TAMANHO_MEMORIA);
    gerenciador->listaBloqueados = (int *)malloc(sizeof(int) * TAMANHO_MEMORIA);

    gerenciador->tamListaProntos = 0;
    gerenciador->tamListaBloqueados = 0;
    gerenciador->intrucaoAtual = 0;

    gerenciador->conjuntoInstrucoes = arquivoEntrada;
    //gerenciador->TabelaProcessos.listaProcessos[0] = inicializaProcesso(arquivoEntrada);

    inicializarTabelaProcessos(&(gerenciador->TabelaProcessos));
    inserirTabelaProcessos(inicializaProcesso(arquivoEntrada, pid, 0, 0), &(gerenciador->TabelaProcessos));

    gerenciador->listaProntos[0] = 0; // index do primeiro processo criado
    gerenciador->Execucao = -1; // nenhum processo em execução
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
                case 'B':
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



void comecaExecucao(GerenciadorProcessos *gerenciador) {

    // Inicializa o processo atual
    for (int i = 0; i < MAX_PROCESSOS; i++)
    {
        if (gerenciador->TabelaProcessos.listaProcessos[i] != NULL && i == gerenciador->listaProntos[0])
        {
            gerenciador->cpu.processoEmExecucao = gerenciador->TabelaProcessos.listaProcessos[i];
            gerenciador->TabelaProcessos.listaProcessos[i]->EstadosProcesso = Execucao;
            gerenciador->listaProntos[0] = -1;
            break;
        }
    }

    gerenciador->cpu.contadorPrograma = gerenciador->cpu.processoEmExecucao->PC;
    gerenciador->Execucao = gerenciador->cpu.processoEmExecucao->ID_Processo;

    if(gerenciador->cpu.processoEmExecucao->memoria != NULL){
        gerenciador->cpu.memoriaVect = gerenciador->cpu.processoEmExecucao->memoria;
    } else {
        Instrucao instrucao;
        instrucao = processarLinhaEspecifica(gerenciador->cpu.processoEmExecucao->conjuntoInstrucoes, (gerenciador->cpu.contadorPrograma + 1));
        processarComando(gerenciador, instrucao);
    }/* code */
}

void executarProcessoAtual(GerenciadorProcessos *gerenciador)
{

    // Executa o processo atual
    Instrucao instrucao;
    
    if(gerenciador->Execucao != -1){
        instrucao = processarLinhaEspecifica(gerenciador->cpu.processoEmExecucao->conjuntoInstrucoes, (gerenciador->cpu.contadorPrograma + 1));
        processarComando(gerenciador, instrucao);
    } else {
        instrucao = processarLinhaEspecifica(gerenciador->conjuntoInstrucoes, (gerenciador->intrucaoAtual + 1));
        processarComando(gerenciador, instrucao);
    }
    
    processarComando(gerenciador, instrucao);
}