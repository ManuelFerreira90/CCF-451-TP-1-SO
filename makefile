CC = gcc
CFLAGS = -Wall -Wextra -std=c99
DEPS = include/control_process.h include/process_manager.h include/utils.h include/simulated_process.h
OBJ = main.o control_process.o utils.o simulated_process.o
PM_OBJ = process_manager.o utils.o

# Regra para compilar os arquivos .o
%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Regra principal para compilar o executável main
main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# Regra para compilar o gerenciador de processos
process_manager: $(PM_OBJ)
	$(CC) -o process_manager $^ $(CFLAGS) # Não precisa do ./src/

clean:
	rm -f *.o main process_manager
