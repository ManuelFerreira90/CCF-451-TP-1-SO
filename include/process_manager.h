#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

void handle_command(char *cmd);
void handle_N(int process_id, int n);
void handle_D(int process_id, int x);
void handle_F(int process_id);
void handle_R(int process_id);


#endif
