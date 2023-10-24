#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#include "header.h"

int main(){

	pid_t pid;
	int i;
	
	//Prendo la key della coda
	key_t queue = ftok(FTOK_PATH_Q,FTOK_CHAR_Q);
	
	//Prendo il descrittore
	int id_queue = msgget(queue,IPC_CREAT|0664);
	
	if (id_queue<0){
	
		perror("Mssget fallita!");
		exit(1);
		
	}
	
	//Forko
	
	//P1
	pid = fork();
	
	if (pid == 0){
	
		execl("./p1","./p1",NULL);
		
		//QUA NON DEVO ARRIVARCI!
		perror("Exec fallita!");
		exit(1);
	
	}
	
	else if (pid<0){
	
		perror("Fork fallita!");
		exit(1);
		
	}
	
	//P2
	pid = fork();
	
	if (pid == 0){
	
		execl("./p2","./p2",NULL);
		
		//QUA NON DEVO ARRIVARCI!
		perror("Exec fallita!");
		exit(1);
	
	}
	
	else if (pid<0){
	
		perror("Fork fallita!");
		exit(1);
		
	}

	//P3
	pid = fork();
	
	if (pid == 0){
	
		execl("./p3","./p3",NULL);
		
		//QUA NON DEVO ARRIVARCI!
		perror("Exec fallita!");
		exit(1);
	
	}
	
	else if (pid<0){
	
		perror("Fork fallita!");
		exit(1);
		
	}

	//ATTENDO LA TERMINAZIONE DEI FIGLI
	for (i = 0;i<3;i++){
		int riswait=wait(NULL);
		printf("[PADRE] Figlio %d terminato!\n",riswait);
	}
	
	//PULISCO 
	msgctl(id_queue,IPC_RMID,0);
	
	return 0;

}
