#define BUFFER 1 // Lendo um caractere por vez

#include "../headers/gerenciador_de_processos.h"
#include "../headers/processoControle.h"

// TODO: @Tarik 1. Implementar função para imprimir cada processo com usa memória e tempo de CPU
// TODO: @Tarik 2. Implementar função para as filas de prioridades ou fila de round robin, confome a escolha inicial do escalonamento

int processoControle()
{
    int fd[2], fd_filho[2]; /* Descritores de arquivo para o Pipe */
    pid_t pid;              /* Variável para armazenar o PID */
    int file_fd;            /* Descritor de arquivo para o init.txt */
    char buffer[BUFFER];
    ssize_t bytes_read;
    char escolha;
    char stringEntrada[100];
    char stringSaida[100];
    FILE *entrada = stdin; // Por padrão, lê da entrada padrão

    /* Criando o Pipe */
    if (pipe(fd) < 0)
    {
        perror("pipe");
        return -1;
    }

    /* Criando o processo filho */
    if ((pid = fork()) < 0)
    {
        perror("fork");
        exit(1);
    }

    /* Processo Pai */
    if (pid > 0)
    {
        /* No pai, ler comandos e escrever no Pipe */
        close(fd[0]); // Fechar a leitura do Pipe no lado do pai

        // Leitura do número de CPUs desejadas
        int numero_CPUS;

        do
        {
            printf("Digite o número de CPUs desejadas: ");
            scanf("%d", &numero_CPUS);
            if (numero_CPUS < 1)
            {
                printf("Número de CPUs inválido. Tente novamente.\n");
            }
        } while (numero_CPUS < 1);

        // Leitura do tipo de escalonamento
        int tipo_escalonamento;
        do
        {
            printf("Escolha o tipo de escalonamento (0: Fila de Prioridades, 1: Round Robin): ");
            scanf("%d", &tipo_escalonamento);
            if (tipo_escalonamento != 0 && tipo_escalonamento != 1)
            {
                printf("Tipo de escalonamento inválido. Tente novamente.\n");
            }
        } while (tipo_escalonamento != 0 && tipo_escalonamento != 1);
        // Enviar o número de CPUs e o tipo de escalonamento para o processo filho
        write(fd[1], &numero_CPUS, sizeof(int));
        write(fd[1], &tipo_escalonamento, sizeof(int));

        int entradaUsu;
        do
        {
            sleep(2);
            printf("Escolha o tipo de entrada (1: terminal, 2: arquivo): ");
            scanf("%d", &entradaUsu);

            if (entradaUsu == 1 || entradaUsu == 2)
            {
                if (entradaUsu == 1)
                {
                    lerTerminal(stringEntrada);
                }
                else if (entradaUsu == 2)
                {
                    lerArquivo(stringEntrada);
                }
                break;
            }
            else
            {
                printf("Entrada inválida. Tente novamente.\n");
            }

        } while (entradaUsu != 1 || entradaUsu != 2);
        printf("String enviada: %s\n", stringEntrada);
        write(fd[1], stringEntrada, sizeof(stringEntrada));
        close(fd[1]); // Fechar o lado de escrita do Pipe
        wait(NULL);   // Esperar o filho terminar

        exit(0);
    }

    /* Processo Filho */
    else
    {
        if (pipe(fd_filho) < 0)
        {
            perror("pipe");
            return -1;
        }
        char str_recebida[BUFFER + 1]; // +1 para o terminador nulo
        ssize_t bytes_read;

        /* Inicializar o Gerenciador de Processos */
        int unidadeTempo = 0;
        int numero_CPUS, tipo_escalonamento;

        // Ler o número de CPUs e o tipo de escalonamento do pipe
        read(fd[0], &numero_CPUS, sizeof(int));
        read(fd[0], &tipo_escalonamento, sizeof(int));

        GerenciadorProcessos gerenciador;
        iniciarGerenciadorProcessos(&gerenciador, "./entry/input1.txt", getpid(), numero_CPUS, tipo_escalonamento);

        /* No filho, ler do Pipe e processar comandos */
        close(fd[1]); // Fechar a escrita do Pipe no lado do filho

        // Mensagem de depuração
        wait(NULL);
        printf("Filho: Processando comandos...\n");

        while ((bytes_read = read(fd[0], str_recebida, BUFFER)) > 0)
        {
            str_recebida[bytes_read] = '\0'; // Garantir que a string seja terminada

            // Processar comandos com o Gerenciador de Processos
            switch (str_recebida[0])
            {
            case 'U':
                printf("\nUnidade de tempo: %d\n", unidadeTempo);
                if (tipo_escalonamento == 0)
                {
                    // printf("\nEscalonador de Fila de Prioridades\n");
                    escalonadorFilaDePrioridades(&gerenciador);
                }
                else if (tipo_escalonamento == 1)
                {
                    // printf("\nEscalonador Round Robin\n");
                    escalonadorRoundRobin(&gerenciador);
                }
                incrementarTempoCPU(&gerenciador);

                printf("Fim de uma unidade de tempo.\n");
                unidadeTempo++;
                break;
            case 'I':
                printf("\nImprimindo estado atual do sistema.\n");
                // Chamar função do Gerenciador de Processos para imprimir o estado
                for (int i = 0; i < gerenciador.quantidadeCPUs; i++)
                {
                    imprimeCPU(gerenciador.cpus[i], i);
                }
                break;
            case 'M':
            {
                printf("\nImprimindo tempo médio de resposta e finalizando.\n");

                pid_t print_pid = fork();
                if (print_pid == 0)
                {
                    // Processo filho para impressão
                    imprimirTempoMedioProcessos(gerenciador);
                    imprimeTabelaProcessos(&gerenciador.TabelaProcessos);

                    exit(0); // Finaliza o processo de impressão
                }
                else if (print_pid > 0)
                {
                    // Processo pai espera o filho terminar
                    wait(NULL);
                }
                else
                {
                    perror("fork");
                }
                break;
            }
            case ' ':
                break;
            default:
                printf("Comando desconhecido: %c\n", str_recebida[0]);
                break;
            }

            if (str_recebida[0] == 'M')
            {
                break;
            }
        }

        if (bytes_read < 0)
        {
            perror("read");
        }
        close(fd[0]); // Fechar o lado de leitura do Pipe
        exit(0);
    }

    return 0;
}

void lerArquivo(char *retorno)
{
    FILE *arquivo;
    char str[MAX_CMD_LEN];

    // atribua ao arquivo o Controle.txt
    arquivo = fopen("./entry/Controle.txt", "r");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir arquivo\n");
        return;
    }
    else
    {
        while (fscanf(arquivo, "%s", str) != EOF)
        {
            strcat(retorno, str);
            strcat(retorno, " ");
        }

        fclose(arquivo);
    }
}

void lerTerminal(char *retorno)
{
    char comando;
    int i = 0;

    printf("Entre com os comandos:\n");

    do
    {
        scanf(" %c", &comando);
        if (comando >= 97 && comando <= 120)
        {
            comando = comando - 32;
        }
        retorno[i] = comando;
        strcat(retorno, " ");
        i++;
    } while (comando != 'M');

    remove_char(retorno, '\n');
}

void remove_char(char *str, char c)
{
    char *pr = str, *pw = str;

    while (*pr)
    {
        *pw = *pr++;
        pw += (*pw != c);
    }

    *pw = '\0';
}
