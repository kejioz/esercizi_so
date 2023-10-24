#include "header.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

int main(){

	int i;
	pid_t pid;
	
	//Buffer
	int id_shm = shmget(IPC_PRIVATE,sizeof(ProdCons),IPC_CREAT|0664);
	ProdCons* pc = (ProdCons*) shmat(id_shm,0,0);
	inizializza(pc);
	
	//Produttori
	for (i=0;i<NUM_PRODUTTORI;i++){
	
		pid = fork();
		
		if (pid == 0){
			srand(i*getpid());
			produci(pc);
			exit(0);
		
		}
		
	}
	
	//Consumatori
	for (i=0;i<NUM_CONSUMATORI;i++){
	
		pid = fork();
		
		if (pid == 0){
			srand(i*getpid());
			consuma(pc);
			exit(0);
		
		}
		
	}
	
	//Wait
	for (i=0;i<NUM_PRODUTTORI+NUM_CONSUMATORI;i++){
		wait(NULL);
		printf(" /// P R O C %d  T E R M I N A T O \n",i+1);
	}
	
	pulisci(pc,id_shm);

	return 0;

}
