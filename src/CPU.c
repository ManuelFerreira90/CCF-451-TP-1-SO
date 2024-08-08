#include "../headers/CPU.h"

void printTableBorder()
{
    for (int i = 0; i < TABLE_WIDTH; i++)
    {
        printf("=");
    }
    printf("\n");
}

void imprimeCPU(CPU cpu, int index)
{
    printf("\n");
    printTableBorder();
    printf("| %-32s %-3d |\n", "CPU Status ID", index); // Exibe o índice da CPU
    printTableBorder();
    
    const char *estadoCPU = (cpu.processoEmExecucao != NULL) ? "Ocupada" : "Vazia";
    printf("| %-25s | %-8s |\n", "Estado CPU", estadoCPU);
    printTableBorder();
    
    printf("| %-25s | %8d |\n", "Contador de programa", cpu.contadorPrograma);
    printTableBorder();
    printf("| %-25s | %8d |\n", "Fatia de tempo", cpu.fatiaTempo.valor);
    printTableBorder();
    printf("| %-36s |\n", "Memória atual na CPU");
    printTableBorder();

    if (cpu.memoriaVect != NULL)
    {
        printf("| %-15s | %-18s |\n", "Index", "Valor");
        printTableBorder();
        for (int i = 0; i < cpu.quantidadeInteiros; i++)
        {
            printf("|  %14d | %18d |\n", i, cpu.memoriaVect[i]);
        }
        printTableBorder();
    }
    else
    {
        printf("| %-36s |\n", "NULL");
        printTableBorder();
    }
    printf("\n\n");
}
