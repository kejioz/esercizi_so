#include "processini.h"
#include "semafori.h"

#include <sys/wait.h>
#include <limits.h>
#include <unistd.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int inizializza_semafori(){

	//Key
	key_t k_sem=IPC_PRIVATE;
	
	//Faccio semget per il set di semafori. Uso un solo semaforo sia per race condition che per mtx padrechild
	int sem_id = semget(sem_id,1,IPC_CREAT|0664);
	
	if (sem_id < 0){
		
		printf("Errore semget\n");
		exit(1);
	
	}
	
	//Adesso inizializzo il semaforo ad uno
	semctl(sem_id,0,SETVAL,1);

	//Ritorno il descrittore del semaforo
	return sem_id;

}

void figlio(int* vettore, int* buffer, int sem_id, int elemento_iniziale, int qta_elementi){

	//Ricerco il minimo nella MIA SEZIONE. Non credo che qui ci voglia mutua esclusione
	int minimo = INT_MAX;
	
	for (int i = elemento_iniziale; i < elemento_iniziale + qta_elementi;i++){
	
		if (vettore[i] < minimo){
		
			minimo = vettore[i];
		
		}
	
	
	}
	
	printf("Figlio: Il minimo locale tra %d e %d è %d\n",elemento_iniziale,elemento_iniziale+qta_elementi,minimo);

	//Una volta finito di cercare nella MIA sezione vado a controllare il buffer. Qui ho sezione critica
	
	//Entro nella sezione critica
	Wait_Sem(sem_id,0);
	
	if (*(buffer)>minimo){
		
		int vecchioval = *buffer;
		*buffer = minimo;
		printf("Figlio: Il vecchio valore del buffer %d è stato sostituito con %d\n",vecchioval,minimo);
	
	}
	
	//Esco dalla sezione critica
	Signal_Sem(sem_id,0);
	
}

void padre(int* buffer, int sem_id){

	//Attesa terminazione figli
	printf("Padre: aspetto che i miei figli terminino...\n");
	sleep(2);
	
	for (int i=0;i<NUM_PROCESSI;i++){
		wait(0);
		printf("Figlio %d terminato! \n",i+1);
	}
	
	//Lettura dal buffer in mutua esclusione (PERCHÈ!!?)
	Wait_Sem(sem_id,0);
	
	printf("Padre: Il valore minimo assoluto è %d\n",*buffer);
	

}
