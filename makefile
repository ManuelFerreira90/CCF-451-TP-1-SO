all:	
	gcc main.c ./src/utils.c ./src/control_process.c ./src/simulated_process.c -o exec
	gcc ./src/process_manager.c -o ./bin/process_manager
	
