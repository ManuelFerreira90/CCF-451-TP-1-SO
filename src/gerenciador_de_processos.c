#include "tempo.c"


typedef struct CPU{
    int contadorPrograma; //Valor atual do contadorPrograma
    int * memoriaVect;  //Vetor de Memória
    Tempo fatiaTempo;
    Tempo tempoUsado;
    int quantidadeInteiros;
    



} CPU;


typedef struct GerenciadorProcessos{

    CPU cpu;
    Tempo tempoAtual;
} GerenciadorProcessos;







void iniciarVetorMemoria(GerenciadorProcessos * gerenciador){
        gerenciador->cpu.memoriaVect = (int *)malloc(gerenciador->cpu.quantidadeInteiros * sizeof(int));
    for(int i = 0; i < gerenciador->cpu.quantidadeInteiros; i++) {
            gerenciador->cpu.memoriaVect[i] = 0;
        }
}
void iniciarCPU(GerenciadorProcessos *gerenciador) {
    inicializarTempo(&(gerenciador->cpu.fatiaTempo));
    inicializarTempo(&(gerenciador->cpu.tempoUsado));
    
    gerenciador->cpu.contadorPrograma = 0;

    iniciarVetorMemoria(gerenciador);
}

void iniciarGerenciadorProcessos(GerenciadorProcessos * gerenciador){
    iniciarCPU(&(gerenciador->cpu));
    
    inicializarTempo(&gerenciador->tempoAtual);
    
    gerenciador->cpu.quantidadeInteiros = 5;
}

void imprimeCPU(CPU cpu) {
    printf("CPU:");
    printf("\nContador de programa: %d\n", cpu.contadorPrograma);
    printf("\nFatia de tempo: %d\n", cpu.fatiaTempo.valor);
    printf("\nMemoria atual na CPU: ");
    
    if(cpu.memoriaVect != NULL) {
        printf("\n");
        for(int i = 0; i < cpu.quantidadeInteiros; i++) {
         printf("index: %d | valor: %d\n", i, cpu.memoriaVect[i]);
        }
    } else {
        printf("NULL\n");
    }

}

int main() {
    GerenciadorProcessos gerenciador;
    iniciarGerenciadorProcessos(&gerenciador);
    imprimeCPU(gerenciador.cpu);

    return 0;
}