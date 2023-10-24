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
	int id_sem = semget(IPC_PRIVATE,2,IPC_CREAT|0664);
	semctl(id_sem,MUTEXL,SETVAL,1);
	semctl(id_sem,SYNCH,SETVAL,1);
	
	//Buffer
	int id_shm = shmget(IPC_PRIVATE,sizeof(LettScritt),IPC_CREAT|0664);
	LettScritt* ls = (LettScritt*) shmat(id_shm,0,0);
	inizializza(ls);
	
	//Lettori
	for (i=0;i<NUM_LETTORI;i++){
	
		pid = fork();
		
		if (pid == 0){
			srand(i*getpid());
			lettore(id_sem,ls);
			exit(0);
		
		}
		
	}
	
	//Scrittori
	for (i=0;i<NUM_SCRITTORI;i++){
	
		pid = fork();
		
		if (pid == 0){
			srand(i*getpid());
			scrittore(id_sem,ls);
			exit(0);
		
		}
		
	}
	
	//Wait
	for (i=0;i<NUM_LETTORI+NUM_SCRITTORI;i++){
		wait(NULL);
		printf(" /// P R O C %d  T E R M I N A T O \n",i+1);
	}
	
	pulisci(id_sem,id_shm);

	return 0;

}
