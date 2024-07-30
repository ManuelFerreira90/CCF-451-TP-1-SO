all:
	gcc -o exe ./src/tempo.c  ./src/processo_de_controle.c ./src/gerenciador_de_processos.c
	./exe