all:
	 gcc main.c ./src/calcularMetricas.c ./src/escalonadorFcfs.c ./src/escalonadorRR.c ./src/escalonadorSjf.c ./src/processo.c -o exec
	 ./exec
