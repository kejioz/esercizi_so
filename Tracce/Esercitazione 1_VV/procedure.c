#include "header.h"
#include "semafori.h"

void IniziaLettura(int sem_id,LettScritt* ls){

	//La prima cosa che devo fare è aumentare il contatore del numero di lettori
	Wait_Sem(sem_id,MUTEXL);
	
		//Una volta che ho acquisito il lock sul semaforo dei lettori aumento il numero
		ls->numero_lettori = ls->numero_lettori +1;
		printf("[SISTEMA] E'entrato un nuovo lettore\n");
		
	//Check se sono il primo lettore
	if (ls->numero_lettori == 1){
		
		//Se sono il primo lettore, devo mettere in wait gli scrittori
		printf("[LETTORE] Sono il primo lettore. Faccio attendere gli scrittori\n");
		Wait_Sem(sem_id,SYNCH);
	
	}
	
	//Rilascio il semaforo per aumentare il numero di lettori
	Signal_Sem(sem_id,MUTEXL);
	
}

void FineScrittura(int sem_id){

	//Devo solo fare la signal synch
	Signal_Sem(sem_id,SYNCH);
	printf("[SCRITTORE] Ho finito la scrittura.\n");

}

void IniziaScrittura(int sem_id){

	//Devo solo fare la wait sul synch
	Wait_Sem(sem_id,SYNCH);
	printf("[SCRITTORE] Sono finalmente entrato. Posso scrivere.\n");

}

void FineLettura(int sem_id,LettScritt* ls){

	//La prima cosa che devo fare è decrementare il contatore del numero di lettori
	Wait_Sem(sem_id,MUTEXL);
	
		//Una volta che ho acquisito il lock sul semaforo dei lettori decremento
		ls->numero_lettori = ls->numero_lettori -1;
		printf("[SISTEMA] E' uscito un lettore\n");
		
	//Check se sono l'ultimo lettore
	if (ls->numero_lettori == 0){
	
		//Se sono l'ultimo lettore, posso sbloccare gli scrittori in attesa
		printf("[LETTORE] Sono l'ultimo lettore. Posso far entrare gli scrittori\n");
		Signal_Sem(sem_id,SYNCH);
	
	}
	
	//Rilascio il semaforo per la variabile
	Signal_Sem(sem_id,MUTEXL);

}
