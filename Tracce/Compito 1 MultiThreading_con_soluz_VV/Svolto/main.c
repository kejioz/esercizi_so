#include <sys/msg.h>
#include <sys/types.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "header.h"

int main(int agrc, char*argv) {

	key_t msg_key = ftok(".", '.');
	int msgid = msgget(msg_key, IPC_CREAT|0664);
	
	if (msgid <0) {perror("errore coda"); exit(1);}
	
	int i;
	pid_t pid_c, pid_s;
	
	for (i=0; i<NUM_CLIENT; i++) {
	
		pid_c = fork();
		
		if (pid_c==0) {
		
			printf("[%d] Figlio client.\n", getpid());
			
			execve("./main_client", NULL, NULL);
			
			exit(0);
		}
	}
	
	pid_s = fork();
	
	if (pid_s == 0) {
	
		printf("[%d] Figlio server.\n", getpid());
	
		execve("./main_server", NULL, NULL);
	
		exit(0);	
	}
		
	
	for (i=0; i<NUM_CLIENT+1; i++) {
		wait(NULL);
	}
	
	
	msgctl(msgid, IPC_RMID, 0);
	return 0;
}
