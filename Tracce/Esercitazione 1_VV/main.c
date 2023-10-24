#include "procedure.h"
#include "semafori.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h> 

int main(){
	
	int i;
	pid_t pid;
	//Prendo la key per la shared memory
	key_t key_shmem = ftok(FTOK_PATH,FTOK_CHAR_SHM);
	
	//Prendo il descrittore della shared memory
	int id_shm = shmget(key_shmem,sizeof(LettScritt),IPC_CREAT|0664);
	
		//Check su ritorno della get della shared memory
		if (id_shm < 0 ){
		
			printf("Errore allocazione shared memory\n");
			exit(1);
		
		}
		
	//Prendo il puntatore della shared memory
	LettScritt* ls = (LettScritt*) shmat(id_shm,0,0);
	
		//Check sul puntatore
		if (ls == (void*) -1){
		
			printf("Errore attach alla shared memory\n");
			exit(1);
		
		}
		
	//Prendo la key per il semaforo
	key_t key_sem = ftok(FTOK_PATH,FTOK_CHAR_SEM);
	
	//Prendo il descrittore del semaforo
	int id_sem = semget(key_sem,2, IPC_CREAT|0664);
	
		//Check su ritorno della get del set del semaforo
		if (id_sem < 0 ){
		
			printf("Errore allocazione semaforo\n");
			exit(1);
		
		}
		
	//Inizializzo i semafori
	semctl(id_sem,MUTEX,SETVAL,1);
	semctl(id_sem,SYNCH,SETVAL,1);
	
	//Creo i processi figli
	srand(time(NULL)*getpid());
	
	pid = fork();
	
	if (pid == 0){
	
		//Generatore
	
	}
	
	pid = fork();
	
	if (pid == 0){
	
		//Elaboratore
	
	}
	
	pid = fork();
	
	if (pid == 0){
	
		//Elaboratore
	
	}
	
	//Join 
	for (i=0;i<3;i++){
		wait(NULL);
		printf("Processo %d terminato\n",i);
	}
	
	//Deallocazione risorse
	shmctl(id_shmem,IPC_RMID,0);
	semctl(id_sem,0,IPC_RMID);
	
	return 0;

}
