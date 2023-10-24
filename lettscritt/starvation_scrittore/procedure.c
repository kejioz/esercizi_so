#include "procedure.h"
#include "semafori.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <time.h>

void InizioLettura(int sem, Buffer* buf){

	//Devo aumentare il numero di lettori agendo uslla variabile numlettori in mutua esclusione.
	Wait_Sem(sem,MUTEXL);
	
		//Sono nella sezione critica: aumento il numero di lettori
		buf->numlettori = buf->numlettori+1;
		
		//Se dopo la signal sono il primo lettore, vuol dire che chi vuole scrivere deve waitare
		if (buf->numlettori == 1){
		
			//Faccio waitare gli scrittori sul semaforo synch (gestione cooperazione)
			Wait_Sem(sem,SYNCH);
		
		}
	
	//Posso uscire dalla sezione critica (gestione concorrenza tra lettori)
	Signal_Sem(sem,MUTEXL);

}


void FineLettura(int sem, Buffer* buf){

	//Devo decrementare il numero di lettori. Gestisco la concorrenza in mutua esclusione
	Wait_Sem(sem,MUTEXL);
	
		//Sono in sez critica : decremento numlettori
		buf->numlettori = buf->numlettori-1;
		
		//Se dopo il decremento numlettori è 0 vuol dire che ero l'ultimo lettore e posso 
		//Svegliare gli scrittori che erano in wait sul semaforo synch per la Cooperazione
		if (buf->numlettori == 0 ){
			
			Signal_Sem(sem,SYNCH);
			
		}
		
	//Posso uscire dalla sezione critica, ho correttamente decrementato tutto e segnalato
	Signal_Sem(sem,MUTEXL);

}

void InizioScrittura(int sem){
	
	//Devo soltanto gestire la cooperazione tra scrittori e lettori
	Wait_Sem(sem,SYNCH);
	
}

void FineScrittura(int sem){
	
	//Idem
	Signal_Sem(sem,SYNCH);

}


void Lettore(int sem, Buffer* buf){

	//Devo fare un Inizio Lettura ed un FineLettura per gestire concorrenza. Sono wrapper-concurr
	InizioLettura(sem,buf);
	
	//Aspetto un secondo e leggo
	sleep(1);
	printf("[LETTORE] Letto valore %ld. Numero di lettori: %d\n",buf->messaggio,buf->numlettori);
	
	//FineLettura
	FineLettura(sem,buf);

}

void Scrittore(int sem, Buffer* buf){

	//Devo fare un Inizio Scrittura ed un Fine Scrittura per gestire conc. SEmpre wrapper
	InizioScrittura(sem);
	
	//Aspetto un secondo e scrivo
	sleep(1);
	srand(time(NULL));
	buf->messaggio = rand() % 250;
	printf("[SCRITTORE] Scritto nel buffer valore %ld\n",buf->messaggio);
	
	//FIneScrittura
	FineScrittura(sem);
	
}

