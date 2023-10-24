#include "procedure.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(){

	printf("[STUDENTE] IN ESECUZIONE...\n");
	//Chiavi per shm e per semaforo
	key_t key_shm = ftok(PATH,ID);
	key_t key_sem = ftok(PATH,ID_SEM);
	
	//Prendo i descrittori di shmemory e semaforo
	int id_shm = shmget(key_shm,sizeof(esame_t),IPC_CREAT|0664);
	int id_sem = semget(key_sem,3,IPC_CREAT|0664);
	
	if (id_shm < 0 || id_sem <0){
	
		printf("Errore semget / shmget \n");
		exit(1);
	
	}
	
	//Puntatore alla shared memory
	esame_t* esame = (esame_t*) shmat(id_shm,0,0);
	if (esame == (void*) -1){
	
		printf("Errore shmat\n");
		exit(1);
		
	}

	//Posso chiamare la funzione strudente
	Studente(id_sem,esame);
	
	return 0;

}
