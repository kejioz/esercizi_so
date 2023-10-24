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
	
	//Semafori
	int id_sem = semget(IPC_PRIVATE,4,IPC_CREAT|0664);
	semctl(id_sem,SPAZIO_DISPONIBILE,SETVAL,DIM);
	semctl(id_sem,MESSAGGIO_DISPONIBILE,SETVAL,0);
	semctl(id_sem,MUTEX_P,SETVAL,1);
	semctl(id_sem,MUTEX_C,SETVAL,1);
	
	//Buffer
	int id_shm = shmget(IPC_PRIVATE,sizeof(ProdCons),IPC_CREAT|0664);
	ProdCons* pc = (ProdCons*) shmat(id_shm,0,0);
	inizializza(pc);
	
	//Produttori
	for (i=0;i<NUM_PRODUTTORI;i++){
	
		pid = fork();
		
		if (pid == 0){
			srand(i*getpid());
			produci(id_sem,pc);
			exit(0);
		
		}
		
	}
	
	//Consumatori
	for (i=0;i<NUM_CONSUMATORI;i++){
	
		pid = fork();
		
		if (pid == 0){
			srand(i*getpid());
			consuma(id_sem,pc);
			exit(0);
		
		}
		
	}
	
	//Wait
	for (i=0;i<NUM_PRODUTTORI+NUM_CONSUMATORI;i++){
		wait(NULL);
		printf(" /// P R O C %d  T E R M I N A T O \n",i+1);
	}
	
	pulisci(id_sem,id_shm);

	return 0;

}
