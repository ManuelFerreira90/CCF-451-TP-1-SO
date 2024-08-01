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

void iniciarGerenciadorProcessos(GerenciadorProcessos *gerenciador, char* arquivoEntrada)
{
    // Inicializa a CPU e outras partes do gerenciador
    iniciarCPU(gerenciador);
    inicializarTempo(&gerenciador->tempoAtual);
    gerenciador->cpu.quantidadeInteiros = 5;
    inicializarTabelaProcessos(&(gerenciador->TabelaProcessos));

    // Abre o arquivo de entrada
    FILE *file = fopen(arquivoEntrada, "r");
    if (!file) {
        fprintf(stderr, "Erro ao abrir o arquivo %s\n", arquivoEntrada);
        exit(EXIT_FAILURE);
    }

    char linha[MAX_CMD_LEN];
    ProcessoSimulado *processoAtual = NULL;
    Instrucao *instrucoes = NULL;
    int quantidadeInstrucoes = 0;

    // Lê o arquivo linha por linha
    while (fgets(linha, sizeof(linha), file)) {
        char comando[MAX_CMD_LEN];
        int valor1, valor2;

        // Remove a quebra de linha no final
        linha[strcspn(linha, "\n")] = '\0';

        if (sscanf(linha, "N %d", &valor1) == 1) {
            // Cria um novo processo com 'N'
            if (processoAtual) {
                processoAtual->instrucoes = malloc(quantidadeInstrucoes * sizeof(Instrucao));
                for (int i = 0; i < quantidadeInstrucoes; i++) {
                    processoAtual->instrucoes[i] = instrucoes[i];
                }
                free(instrucoes);
                quantidadeInstrucoes = 0;
            }
            processoAtual = &gerenciador->TabelaProcessos.lista_processos[gerenciador->TabelaProcessos.ultimoProcessoIndex++];
            processoAtual->ID_Processo = gerenciador->TabelaProcessos.ultimoProcessoIndex - 1;
            processoAtual->ID_Processo_Pai = -1; // Processo inicial não tem pai
            processoAtual->PC = 0;
            processoAtual->EstadosProcesso = Pronto;
            processoAtual->prioridade = 0;
            processoAtual->quantidadeInteiros = valor1;
            processoAtual->memoria = calloc(valor1, sizeof(int));
            instrucoes = NULL; // Inicializa a lista de instruções
        } else if (sscanf(linha, "D %d", &valor1) == 1) {
            // Define a variável para 0 com 'D'
            if (processoAtual) {
                // Adiciona a instrução 'D'
                quantidadeInstrucoes++;
                instrucoes = realloc(instrucoes, quantidadeInstrucoes * sizeof(Instrucao));
                instrucoes[quantidadeInstrucoes - 1].funcao = 'D';
                instrucoes[quantidadeInstrucoes - 1].valor = valor1;
            }
        } else if (sscanf(linha, "V %d %d", &valor1, &valor2) == 2) {
            // Define a variável com 'V'
            if (processoAtual) {
                // Adiciona a instrução 'V'
                quantidadeInstrucoes++;
                instrucoes = realloc(instrucoes, quantidadeInstrucoes * sizeof(Instrucao));
                instrucoes[quantidadeInstrucoes - 1].funcao = 'V';
                instrucoes[quantidadeInstrucoes - 1].valor = valor1;
            }
        } else if (sscanf(linha, "A %d %d", &valor1, &valor2) == 2) {
            // Adiciona 'A' a variável
            if (processoAtual) {
                // Adiciona a instrução 'A'
                quantidadeInstrucoes++;
                instrucoes = realloc(instrucoes, quantidadeInstrucoes * sizeof(Instrucao));
                instrucoes[quantidadeInstrucoes - 1].funcao = 'A';
                instrucoes[quantidadeInstrucoes - 1].valor = valor1;
            }
        } else if (sscanf(linha, "S %d %d", &valor1, &valor2) == 2) {
            // Subtrai 'S' da variável
            if (processoAtual) {
                // Adiciona a instrução 'S'
                quantidadeInstrucoes++;
                instrucoes = realloc(instrucoes, quantidadeInstrucoes * sizeof(Instrucao));
                instrucoes[quantidadeInstrucoes - 1].funcao = 'S';
                instrucoes[quantidadeInstrucoes - 1].valor = valor1;
            }
        } else if (sscanf(linha, "F %d", &valor1) == 1) {
            // Cria um novo processo filho com 'F'
            if (processoAtual) {
                processoAtual->instrucoes = malloc(quantidadeInstrucoes * sizeof(Instrucao));
                for (int i = 0; i < quantidadeInstrucoes; i++) {
                    processoAtual->instrucoes[i] = instrucoes[i];
                }
                free(instrucoes);
                quantidadeInstrucoes = 0;
            }
            ProcessoSimulado *processoFilho = &gerenciador->TabelaProcessos.lista_processos[gerenciador->TabelaProcessos.ultimoProcessoIndex++];
            processoFilho->ID_Processo = gerenciador->TabelaProcessos.ultimoProcessoIndex - 1;
            processoFilho->ID_Processo_Pai = processoAtual->ID_Processo;
            processoFilho->PC = processoAtual->PC + 1;
            processoFilho->EstadosProcesso = Pronto;
            processoFilho->quantidadeInteiros = processoAtual->quantidadeInteiros;
            processoFilho->memoria = malloc(processoFilho->quantidadeInteiros * sizeof(int));
            memcpy(processoFilho->memoria, processoAtual->memoria, processoFilho->quantidadeInteiros * sizeof(int));
            processoFilho->instrucoes = NULL; // Inicializa a lista de instruções
            processoAtual = processoFilho;
        } else if (sscanf(linha, "R %s", comando) == 1) {
            // Lê um novo arquivo de instruções com 'R'
            // (Para simplificação, a implementação do tratamento de arquivos externos é omitida)
        } else if (strcmp(linha, "T") == 0) {
            // Finaliza o processo de leitura e inicialização
            if (processoAtual) {
                processoAtual->instrucoes = malloc(quantidadeInstrucoes * sizeof(Instrucao));
                for (int i = 0; i < quantidadeInstrucoes; i++) {
                    processoAtual->instrucoes[i] = instrucoes[i];
                }
                free(instrucoes);
                quantidadeInstrucoes = 0;
            }
            break; // Encerra a leitura do arquivo
        } else {
            fprintf(stderr, "Comando desconhecido: %s\n", linha);
        }
    }

    imprimirInstrucoes(processoAtual);

    // Fecha o arquivo
    fclose(file);
}


void imprimirInstrucoes(ProcessoSimulado *processo) {
    printf("Instruções do Processo %d:\n", processo->ID_Processo);
    for (int i = 0; i < processo->quantidadeInteiros; i++) {
        printf("%c %d\n", processo->instrucoes[i].funcao, processo->instrucoes[i].valor);
    }
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