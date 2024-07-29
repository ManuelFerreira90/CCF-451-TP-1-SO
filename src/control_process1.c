#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 1024

// Simulação do estado do sistema
int system_state = 0;
int total_response_time = 0;
int command_count = 0;

// Função para ler comandos de um arquivo e passá-los para o gerenciador de processos
void read_commands_from_file(int write_fd, FILE *input_file) {
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, input_file)) {
        // Remove o caractere de nova linha, se existir
        buffer[strcspn(buffer, "\n")] = 0;

        // Verifica o comando e envia para o gerenciador de processos
        if (strcmp(buffer, "U") == 0) {
            write(write_fd, "U\n", 2);
        } else if (strcmp(buffer, "I") == 0) {
            write(write_fd, "I\n", 2);
        } else if (strcmp(buffer, "M") == 0) {
            write(write_fd, "M\n", 2);
            break;  // Comando M finaliza o sistema, portanto, saímos do loop
        } else {
            printf("Comando inválido: %s\n", buffer);
        }
    }
    close(write_fd);
}

// Função para ler comandos manualmente e passá-los para o gerenciador de processos
// void read_commands_manually(int write_fd) {
//     char command;
//     while (1) {
//         printf("Escolha um comando:\n");
//         printf("1: U - Fim de uma unidade de tempo.\n");
//         printf("2: I - Imprime o estado atual do sistema.\n");
//         printf("3: M - Imprime o tempo médio de resposta e finaliza o sistema.\n");
//         printf("Comando: ");
//         scanf(" %c", &command);

//         printf("\n"); getchar();

//         switch (command) {
//             case '1':
//                 write(write_fd, "U\n", 2);
//                 break;
//             case '2':
//                 write(write_fd, "I\n", 2);
//                 break;
//             case '3':
//                 write(write_fd, "M\n", 2);
//                 close(write_fd);
//                 return;
//             default:
//                 printf("Comando inválido. Tente novamente.\n");
//                 break;
//         }
//     }
// }

// Função para exibir o menu e obter a escolha do usuário
int display_menu() {
    int choice;
    printf("Escolha uma opção:\n");
    printf("1. Ler comandos de um arquivo\n");
    printf("2. Inserir comandos manualmente\n");
    printf("Opção: ");
    scanf("%d", &choice);
    system("clear");
    return choice;
}

// Função para simular o gerenciador de processos
void process_manager() {
    char buffer[BUFFER_SIZE];
    printf("Escolha um comando:\n");
        printf("U - Fim de uma unidade de tempo.\n");
        printf("I - Imprime o estado atual do sistema.\n");
        printf("M - Imprime o tempo médio de resposta e finaliza o sistema.\n");
        printf("Comando: ");
    while (fgets(buffer, BUFFER_SIZE, stdin)) {
        // Remove o caractere de nova linha, se existir
        buffer[strcspn(buffer, "\n")] = 0;
        if (strcmp(buffer, "U") == 0) {
            system_state++;
            total_response_time += system_state;
            command_count++;
            printf("Unidade de tempo avançada. Estado atual: %d\n\n", system_state);
        } else if (strcmp(buffer, "I") == 0) {
            printf("Estado atual do sistema: %d\n\n", system_state);
        } else if (strcmp(buffer, "M") == 0) {
            if (command_count > 0) {
                double average_response_time = (double)total_response_time / command_count;
                printf("Tempo médio de resposta: %.2f\n\n", average_response_time);
            } else {
                printf("Nenhum comando 'U' executado.\n\n");
            }
            printf("Sistema finalizado.\n");
            return;
        } else {
            printf("Comando inválido: %s\n", buffer);
        }
        
    }
}

int main(int argc, char *argv[]) {
    int pipefd[2];
    pid_t pid;
    FILE *input_file = stdin; // padrão é a entrada padrão
    int choice = display_menu();

    if (choice == 1) {
        // Verifica se um arquivo de entrada foi fornecido como argumento
        if (argc != 2) {
            printf("Uso: %s <nome_do_arquivo>\n", argv[0]);
            return EXIT_FAILURE;
        }
        input_file = fopen(argv[1], "r");
        if (!input_file) {
            perror("Erro ao abrir o arquivo");
            return EXIT_FAILURE;
        }else {
        printf("Opção inválida.\n");
        return EXIT_FAILURE;
        }
    }

    // Cria o pipe
    if (pipe(pipefd) == -1) {
        perror("Erro ao criar o pipe");
        return EXIT_FAILURE;
    }

    // Cria o processo gerenciador de processos
    pid = fork();
    if (pid == -1) {
        perror("Erro ao criar o processo filho");
        return EXIT_FAILURE;
    }

    if (pid == 0) { // Processo filho (gerenciador de processos)
        close(pipefd[1]); // Fecha o lado de escrita do pipe

        // Redireciona a entrada padrão para ler do pipe
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        
    } else { // Processo pai (processo controle)
        close(pipefd[0]); // Fecha o lado de leitura do pipe

        if (choice == 1) {
            // Lê comandos do arquivo e envia para o gerenciador de processos
            read_commands_from_file(pipefd[1], input_file);
        } else if (choice == 2) {
            // Simula o gerenciador de processos
            printf("Insira os comandos manualmente (U, I, M):\n");
            process_manager();
            exit(EXIT_SUCCESS);
            // Lê comandos manualmente e envia para o gerenciador de processos
            // read_commands_manually(pipefd[1]);            
        }

        // Espera pelo término do processo filho
        wait(NULL);

        // Fecha o arquivo de entrada se não for a entrada padrão
        if (input_file != stdin) {
            fclose(input_file);
        }
    }

    return EXIT_SUCCESS;
}
