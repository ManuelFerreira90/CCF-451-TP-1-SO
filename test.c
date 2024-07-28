#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
      execl("/bin/ls","ls","test.c",NULL) ;
      printf ("Eu ainda nao estou morto\n") ;
      exit(0);
} 