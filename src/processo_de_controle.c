#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>  // Para open()
#include <sys/wait.h> // Para wait()
#include <errno.h>

#define BUFFER 256

int main(void)
{
    int fd[2]; /* File descriptors para o Pipe */
    pid_t pid; /* Variável para armazenar o pid */
    int file_fd; /* Descritor de arquivo para o input.txt */

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
        /* No pai, vamos ler o arquivo e escrever no Pipe */
        close(fd[0]); // Fechar a leitura do Pipe no lado do pai

        // Mensagem de depuração
        printf("Pai: Enviando arquivo de entrada...\n");

        // Abrir o arquivo input.txt
        file_fd = open("entry/input.txt", O_RDONLY);
        if (file_fd < 0) {
            perror("open");
            exit(1);
        }

        char buffer[BUFFER];
        ssize_t bytes_read;

        // Ler do arquivo e escrever no Pipe
        while ((bytes_read = read(file_fd, buffer, sizeof(buffer))) > 0) {
            if (write(fd[1], buffer, bytes_read) < 0) {
                perror("write");
                exit(1);
            }
        }

        if (bytes_read < 0) {
            perror("read");
        }

        close(file_fd); // Fechar o descritor de arquivo
        close(fd[1]);   // Fechar o lado de escrita do Pipe
        wait(NULL);     // Esperar pelo filho
        exit(0);
    }
    /* Processo Filho */
    else
    {
        char str_recebida[BUFFER];
        ssize_t bytes_read;

        /* No filho, vamos ler do Pipe e exibir o conteúdo */
        close(fd[1]); // Fechar a escrita do Pipe no lado do filho

        // Mensagem de depuração
        printf("Filho: Lendo o arquivo...\n");

        while ((bytes_read = read(fd[0], str_recebida, sizeof(str_recebida))) > 0) {
            write(STDOUT_FILENO, str_recebida, bytes_read); // Exibir no terminal
        }

        if (bytes_read < 0) {
            perror("read");
        }

        close(fd[0]); // Fechar o lado de leitura do Pipe
        exit(0);
    }

    return 0;
}
