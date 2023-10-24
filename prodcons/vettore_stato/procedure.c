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

void Produttore(struct vettoreStato* p, int sem_id){

	int indice = 0;
	//Il produttore fa la wait su SPAZIO DISPONIBILE innanzitutto per vedere se c'è spazio
	Wait_Sem(sem_id,SPAZIO_DISPONIBILE);
	
	//Poi deve fare la wait su MUTEXP per vedere se già c'è qualcuno che sta producendo
	Wait_Sem(sem_id,MUTEX_P);
	
		sleep(2);
		//Sono dentro la sezione critica. Devo SCORRERE FIN QUANDO NON TROVO VUOT
		while (indice < DIM_BUFFER && p->stato[indice] != BUFFER_VUOTO){
			indice++;
		}
		
		//DEVO AGGIORNARE LO STATO DELL'INDICE A IN USO!!
		p->stato[indice]= BUFFER_INUSO;
		
	//QUA DEVO RILASCIARE IL MUTEX PER I PRODUTTORI !!!!!!
	Signal_Sem(sem_id,MUTEX_P);
		
		
		//Adesso vado a fare la mia produzione e a settarlo pieno
		p->buffer[indice] = rand() % 100;
		printf("[PRODUTTORE] Prodotto %d\n",p->buffer[indice]);
		
		p->stato[indice] = BUFFER_PIENO;
	
	//Segnalo ai consumatori che c'è il messaggio
	Signal_Sem(sem_id,MESSAGGIO_DISPONIBILE);
	
}


void Consumatore(struct vettoreStato* p, int sem_id){

	int indice = 0;
	//Il consumatore fa la wait su MESSAGGIO DISPONIBILE innanzitutto per vedere se c'è almeno un messaggio
	Wait_Sem(sem_id,MESSAGGIO_DISPONIBILE);
	
	//Poi deve fare la wait su MUTEXC per vedere se già c'è qualcuno che sta consumando
	Wait_Sem(sem_id,MUTEX_C);
	
		sleep(2);
		//Sono dentro la sezione critica. Devo SCORRERE FIN QUANDO NON TROVO VUOT
		while (indice < DIM_BUFFER && p->stato[indice] != BUFFER_PIENO){
			indice++;
		}
		
		//DEVO AGGIORNARE LO STATO DELL'INDICE A IN USO!!
		p->stato[indice]= BUFFER_INUSO;
		
	//QUA DEVO RILASCIARE IL MUTEX PER I CONSUMATORI !!!!!!
	Signal_Sem(sem_id,MUTEX_C);
		
		
		//Adesso vado a fare la mia consumazione e a settarlo vuoto
		printf("[CONSUMATORE]Consumato il valore %d\n",p->buffer[indice]);
		
		p->stato[indice] = BUFFER_VUOTO;
	
	//Segnalo ai produttori che c'è lo spazio
	Signal_Sem(sem_id,SPAZIO_DISPONIBILE);

}
