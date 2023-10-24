#include "procedure.h"
#include "semafori.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <time.h>

void Produttore(struct codaCircolare* coda, int sem_id){

	//Il produttore fa la wait su SPAZIO DISPONIBILE innanzitutto per vedere se c'è spazio
	Wait_Sem(sem_id,SPAZIO_DISPONIBILE);
	
	//Poi deve fare la wait su MUTEXP per vedere se già c'è qualcuno che sta producento
	Wait_Sem(sem_id,MUTEX_P);
	
		sleep(2);
		//Sono dentro la sezione critica e posso produrre IN CODA!!
		int val = rand() % 100;
		coda->buffer[coda->coda] = val;
		printf("[PRODUTTORE] Ho prodotto il valore %d\n",coda->buffer[coda->coda]);
		
		//Adesso vado ad aumentare il puntatore alla coda
		coda->coda = (coda->coda +1) % DIM_BUFFER;
		
	//Posso uscire dalla sezione critica facendo la signal prima sulla mutua esclusione e poi sul mess disp
	Signal_Sem(sem_id,MUTEX_P);
	
	//Segnalo ai consumatori che c'è il messaggio
	Signal_Sem(sem_id,MESSAGGIO_DISPONIBILE);
	
}

void Consumatore(struct codaCircolare* coda, int sem_id){

	//Il consumatore fa la wait su MESSAGGIO DISPONIBILE innanzitutto per vedere se c'è mess da consumare
	Wait_Sem(sem_id,MESSAGGIO_DISPONIBILE);
	
	//Poi deve fare la wait su MUTEXC per vedere se già c'è qualcuno che sta consumando
	Wait_Sem(sem_id,MUTEX_C);
	
		sleep(2);
		//Sono dentro la sezione critica e posso consumare IN TESTA!!
		printf("Consumato il valore %d\n",coda->buffer[coda->testa]);
		
		//Adesso vado ad aumentare il puntatore alla testa
		coda->testa = (coda->testa +1) % DIM_BUFFER;
		
	//Posso uscire dalla sezione critica facendo la signal prima sulla mutua esclusione e poi sullo spazio 
	Signal_Sem(sem_id,MUTEX_C);
	
	//Segnalo ai produttori che possono produrre
	Signal_Sem(sem_id,SPAZIO_DISPONIBILE);

}
