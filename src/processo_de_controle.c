#define BUFFER 1 // Lendo um caractere por vez

#include "../headers/gerenciador_de_processos.h"

int main() {
    int fd[2]; /* Descritores de arquivo para o Pipe */
    pid_t pid; /* Variável para armazenar o PID */
    int file_fd; /* Descritor de arquivo para o init.txt */
    char buffer[BUFFER];
    ssize_t bytes_read;
    char escolha;
    FILE *entrada = stdin; // Por padrão, lê da entrada padrão

    /* Criando o Pipe */
    if (pipe(fd) < 0) {
        perror("pipe");
        return -1;
    }

    /* Criando o processo filho */
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    /* Processo Pai */
    if (pid > 0) {
        /* No pai, ler comandos e escrever no Pipe */
        close(fd[0]); // Fechar a leitura do Pipe no lado do pai
        printf("Escolha a entrada (a: entrada padrão, f: arquivo): ");
        escolha = getchar();
        getchar(); // Consome o newline após a escolha

        if (escolha == 'f') {
            // Se a escolha for arquivo, abrir o arquivo init.txt
            file_fd = open("entry/init.txt", O_RDONLY);
            if (file_fd < 0) {
                perror("open");
                exit(1);
            }

            // Mensagem de depuração
            printf("Pai: Enviando comandos do arquivo...\n");

            // Ler do arquivo e escrever no Pipe caractere por caractere
            while ((bytes_read = read(file_fd, buffer, BUFFER)) > 0) {
                if (write(fd[1], buffer, bytes_read) < 0) {
                    perror("write");
                    exit(1);
                }
            }
            close(file_fd);
        } else {
            // Caso contrário, ler da entrada padrão
            printf("Digite os comandos (terminar com M):\n");
            while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER)) > 0) {
                if (write(fd[1], buffer, bytes_read) < 0) {
                    perror("write");
                    exit(1);
                }
                if (buffer[0] == 'M') { // Se encontrar o comando M, parar
                    break;
                }
            }
        }

        close(fd[1]); // Fechar o lado de escrita do Pipe
        wait(NULL);   // Esperar o filho terminar
        exit(0);
    }
    /* Processo Filho */
    else {
        char str_recebida[BUFFER];
        ssize_t bytes_read;

        /* Inicializar o Gerenciador de Processos */
        GerenciadorProcessos gerenciador;
        iniciarGerenciadorProcessos(&gerenciador,"./entry/input1.txt");
        

        /* No filho, ler do Pipe e processar comandos */
        close(fd[1]); // Fechar a escrita do Pipe no lado do filho

        // Mensagem de depuração
        printf("Filho: Processando comandos...\n");

        while ((bytes_read = read(fd[0], str_recebida, BUFFER)) > 0) {
            str_recebida[bytes_read] = '\0'; // Garantir que a string seja terminada
            printf("Comando recebido: %c\n", str_recebida[0]);

            // Processar comandos com o Gerenciador de Processos
            switch (str_recebida[0]) {
                case 'U':
                    printf("Comando 'U' recebido: Fim de uma unidade de tempo.\n");
                    // Chamar função do Gerenciador de Processos para avançar no tempo
                    break;
                case 'I':
                    printf("Comando 'I' recebido: Imprimindo estado atual do sistema.\n");
                    // Chamar função do Gerenciador de Processos para imprimir o estado
                    break;
                case 'M':
                    printf("Comando 'M' recebido: Imprimindo tempo médio de resposta e finalizando.\n");
                    // Chamar função do Gerenciador de Processos para imprimir tempo médio e finalizar
                    break;
                default:
                    printf("Comando desconhecido: %c\n", str_recebida[0]);
                    break;
            }
            
            if (str_recebida[0] == 'M') {
                break;
            }
        }

        if (bytes_read < 0) {
            perror("read");
        }

        close(fd[0]); // Fechar o lado de leitura do Pipe
        exit(0);
    }

    return 0;
}
