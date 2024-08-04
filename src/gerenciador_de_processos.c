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

void bloquearProcessoSimulado(GerenciadorProcessos *gerenciador, int tempo)
{
    // Move o processo atual para a fila de bloqueados
    // if (gerenciador->processoAtual)
    // {
    //     adicionarProcessoBloqueado(gerenciador, gerenciador->processoAtual, tempo);
    //     gerenciador->processoAtual = NULL;
    // }
}

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
}

void processarComando(GerenciadorProcessos *gerenciador, char comando, int valor, int index)
{
    switch (comando)
    {
    case 'N':
        // criar vetor de memória
        gerenciador->cpu.memoriaVect = (int *)malloc(valor * sizeof(int));
        gerenciador->cpu.quantidadeInteiros = valor;
        gerenciador->cpu.contadorPrograma++;
        break;
    case 'D':
        // Declara uma nova variável no processo atual
        // Implementar lógica para declarar uma variável
        comandoD(&(gerenciador->cpu), valor);
        gerenciador->cpu.contadorPrograma++;
        break;
    case 'V':
        // Define o valor de uma variável
        // Implementar lógica para definir o valor
        printf("Definindo valor %d para variável %d\n", valor, index);
        break;
    case 'A':
        // Adiciona valor a uma variável
        // Implementar lógica para adicionar valor
        printf("Adicionando valor %d à variável %d\n", valor, index);
        break;
    case 'S':
        // Subtrai valor de uma variável
        // Implementar lógica para subtrair valor
        printf("Subtraindo valor %d da variável %d\n", valor, index);
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
        // Implementar lógica para substituir o programa
        break;
    default:
        printf("Comando desconhecido: %c\n", comando);
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
    gerenciador->listaProntos = (int *)malloc(sizeof(int) * TAMANHO_MEMORIA);
    gerenciador->listaBloqueados = (int *)malloc(sizeof(int) * TAMANHO_MEMORIA);
    gerenciador->TabelaProcessos.listaProcessos[0] = inicializaProcesso(arquivoEntrada);

    gerenciador->listaProntos[0] = 0; // index do primeiro processo criado
    gerenciador->Execucao = -1; // nenhum processo em execução
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

// ler comandos de um arquivo
void processarLinhaEspecifica(int *valor, int *index, char *comando, const char *caminhoArquivo, int numeroLinha) {
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

            *comando = cmd;

            switch (cmd) {
                case 'N':
                case 'D':
                case 'F':
                    if (sscanf(linha + 2, "%d", &v1) == 1) {
                        *valor = v1;
                        printf("Comando: %c, Valor: %d\n", cmd, *valor);
                    }
                    break;
                case 'A':
                case 'S':
                case 'V':
                    if (sscanf(linha + 2, "%d %d", &v1, &v2) == 2) {
                        *index = v1;
                        *valor = v2;
                        printf("Comando: %c, Index: %d, Valor: %d\n", cmd, *index, *valor);
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

    if(gerenciador->cpu.processoEmExecucao->memoria != NULL){
        gerenciador->cpu.memoriaVect = gerenciador->cpu.processoEmExecucao->memoria;
    } else {
        char comando;
        int valor;
        int index;
        processarLinhaEspecifica(&valor, &index, &comando, gerenciador->cpu.processoEmExecucao->conjuntoInstrucoes, (gerenciador->cpu.contadorPrograma + 1));
        printf("Comando: %c, Valor: %d\n", comando, valor);
        processarComando(gerenciador, comando, valor, index);
    }/* code */
}

void executarProcessoAtual(GerenciadorProcessos *gerenciador)
{
    // Executa o processo atual
    char comando;
    int valor;
    int index;
    processarLinhaEspecifica(&valor, &index, &comando, gerenciador->cpu.processoEmExecucao->conjuntoInstrucoes, (gerenciador->cpu.contadorPrograma + 1));
    
    processarComando(gerenciador, comando, valor, index);
}