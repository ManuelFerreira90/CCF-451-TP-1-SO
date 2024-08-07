all:
	gcc -o exe main.c ./src/tempo.c ./src/Fila.c ./src/processo.c ./src/tabela_de_processos.c ./src/processo_de_controle.c ./src/gerenciador_de_processos.c
	./exe