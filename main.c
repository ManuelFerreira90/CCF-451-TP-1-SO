#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "./include/control_process.h"

int main(int argc, char *argv[]) {
    // Verifica se argumentos foram passados, se necessário
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo_de_entrada>\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("Iniciando o processo de controle com o arquivo: %s\n", argv[1]);

    // Cria o processo de controle
    if (start_control_process() == -1) {
        fprintf(stderr, "Falha ao iniciar o processo de controle\n");
        return EXIT_FAILURE;
    }

    printf("Processo de controle finalizado com sucesso.\n");
    return EXIT_SUCCESS;
}
