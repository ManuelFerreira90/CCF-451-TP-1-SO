#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
      execl("/home/sandrin/Documentos/Github/TP-2_SO/src/process_manager","process_manager","process_manager.o",NULL) ;
      printf ("Eu ainda nao estou morto\n") ;
      exit(0);
} 