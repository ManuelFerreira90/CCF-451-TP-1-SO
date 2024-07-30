#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "./include/control_process.h"

int main(int argc, char *argv[]) {
    // Verifica se argumentos foram passados
    const char *input_file = (argc >= 2) ? argv[1] : NULL;

    printf("Iniciando o processo de controle com o arquivo: %s\n", input_file ? input_file : "entrada padrão");

    // Cria o processo de controle
    if (start_control_process(input_file) == -1) {
        fprintf(stderr, "Falha ao iniciar o processo de controle\n");
        return EXIT_FAILURE;
    }

    printf("Processo de controle finalizado com sucesso.\n");
    return EXIT_SUCCESS;
}
