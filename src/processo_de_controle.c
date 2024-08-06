#define BUFFER 1 // Lendo um caractere por vez

#include "../headers/gerenciador_de_processos.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int fd[2], fd_filho[2]; /* Descritores de arquivo para o Pipe */
    pid_t pid;              /* Variável para armazenar o PID */
    int file_fd;            /* Descritor de arquivo para init.txt */
    char buffer[BUFFER];
    char stringEntrada[100];
    char stringSaida[100];

    ssize_t bytes_read;

    char escolha;

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
        int entradaUsu;
        do
        {
            sleep(2);
            printf("Escolha o tipo de entrada (1): terminal, 2: arquivo): ");

            scanf("%d", &entradaUsu);

            if(entradaUsu == 1 || entradaUsu == 2){
                if(entradaUsu == 1){
                    lerTerminal(stringEntrada);
                } else if(entradaUsu == 2){
                    lerArquivo(stringEntrada);
                }
                break;
            } else {
                printf("Entrada inválida. Tente novamente.\n");
            }




        } while (entradaUsu != 1 || entradaUsu != 2);
        printf("String enviada: %s\n", stringEntrada);
        write(fd[1], stringEntrada, sizeof(stringEntrada) + 2);
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
        printf("Iniciando o Gerenciador de Processos...\n");
        GerenciadorProcessos gerenciador;

        iniciarGerenciadorProcessos(&gerenciador, "./entry/input1.txt", getpid());
        comecaExecucao(&gerenciador);

        /* No filho, ler do Pipe e processar comandos */
        close(fd[1]); // Fechar a escrita do Pipe no lado do filho

        // Mensagem de depuração
        printf("Filho: Processando comandos...\n");

        while ((bytes_read = read(fd[0], str_recebida, BUFFER)) > 0)
        {
            str_recebida[bytes_read] = '\0'; // Garantir que a string seja terminada

            // Processar comandos com o Gerenciador de Processos
            switch (str_recebida[0])
            {
            case 'U':
                printf("Fim de uma unidade de tempo.\n");
                executandoProcessoCPU(&gerenciador);
                break;
            case 'I':
                printf("Imprimindo estado atual do sistema.\n");
                for (int i = 0; i < gerenciador.quantidadeCPUs; i++)
                {
                    imprimeCPU(gerenciador.cpus[i]);
                }
                break;
            case 'M':
                printf("Imprimindo tempo médio de resposta e finalizando.\n");
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

/*
 * Function:  lerArquivo
 *
 * Pega a entrada a partir do arquivo de controle
 *
 * @params:
 * retorno: string lida do arquivo
 *
 *  returns: void
 */
void lerArquivo(char *retorno)
{
    FILE *arquivo;
    char str[2];

    //atribua ao arquivo o Controle.txt
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

/*
 * Function:  lerTerminal
 *
 * Pega a entrada a partir do terminal do usuario
 *
 * @params:
 * retorno: string lida do terminal
 *
 *  returns: void
 */
void lerTerminal(char *retorno)
{
    char comando;
    int i = 0;

    printf("Entre com os comandos:\n");

    do
    {
        scanf("%c", &comando);
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

/*
 * Function:  remove_char
 *
 * Funcao auxiliar que remove um char de uma string
 *
 * @params:
 * str: string que queremos trabalhar
 * c: char que queremos remover da string
 *
 *  returns: void
 */
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