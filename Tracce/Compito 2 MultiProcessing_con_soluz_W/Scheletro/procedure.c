#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "header.h"

void inizializza_vettore(VettoreProdCons * p) {

    /* TBD: Inizializzare la struttura dati "VettoreProdCons" */
    
	//Questo lo devo gestire con il monitor, quindi prima cosa inizializzo il monitor con 2 condition var
	p->testa=0;
	p->coda=0;
	p->elems=0;
	init_monitor(&(p->m),2);
    
    printf("Vettore correttamente ed inizializzato\n");

}

void produci(VettoreProdCons * p, int valore) {

    /* TBD: Implementare la sincronizzazione, mediante
     *      l'algoritmo del vettore circolare di buffer
     */
     
    //Entro nel monitor
    printf("[PRODUTTORE] Provo ad entrare nel monitor per produrre %d...\n",valore);
    enter_monitor(&(p->m));
    
    	printf("[PRODUTTORE] Sono nel monitor...\n");
    	//Sono dentro. Check sulla condition variable. If siccome è signal and wait
    	printf("[PRODUTTORE] Numero di elementi PRE: %d\n",p->elems);
    	if (p->elems == DIM){
    	
    		printf("[PRODUTTORE] Buffer pieno, aspetto...\n");    	
    		//Se il buffer è pieno mi devo mettere in wait
    		wait_condition(&(p->m),CV_PROD);
    		
    		printf("[PRODUTTORE] Mi sono svegliato \n");
    	
    	}

		//Produco in coda
    	p->buffer[p->coda] = valore;
    	
    	//Aumento elems e coda
    	p->coda = (p->coda + 1) % DIM;
    	p->elems = p->elems+1;
    	printf("[PRODUTTORE] Prodotto con successo %d, numero elementi POST: %d \n",valore,p->elems);
    	
    	//Segnalo i consumatori 
    	signal_condition(&(p->m),CV_CONS);
    	printf("[PRODUTTORE] Segnalati i consumatori...\n");
    	
    //Esco dal monitor
    leave_monitor(&p->m);
    printf("[PRODUTTORE] Uscito dal monitor...\n");

}

int consuma(VettoreProdCons * p) {

    int valore;

    /* TBD: Implementare la sincronizzazione, mediante
     *      l'algoritmo del vettore circolare di buffer
     */
     
     printf("[CONSUMATORE] Provo ad entrare nel monitor...\n");
     //Come prima coas devo entrare nel monitor
     enter_monitor(&p->m);
     	
     	printf("[CONSUMATORE] Sono nel monitor...\n");
     	//Sono dentro. Check sulla condition variable
     	printf("[CONSUMATORE] Numero di elementi: %d\n",p->elems);
     	if (p->elems==0){
     	
     		printf("[CONSUMATORE] Buffer vuoto, aspetto...\n");
     		//Se il buffer è vuoto mi devo mettere in wait
     		wait_condition(&p->m,CV_CONS);
     		
     		printf("[CONSUMATORE] Mi sono svegliato \n");
     		
     	
     	}
     	
     	//A questo punto posso prendere il valore, decrementare il nuomero di elementi e aumentare la testa
    	valore = p->buffer[p->testa];
    	printf("[CONSUMATORE] Consumato con successo %d\n",valore);
    	p->elems = p->elems-1;
    	p->testa = (p->testa+1) % DIM;
    	
    	//Segnalo i produttori che ora possono produrre ed esco dal monitor
    	signal_condition(&p->m,CV_PROD);
    	printf("[CONSUMATORE] Segnalati i produttori...\n");
    	
    //Esco dal monitor
    leave_monitor(&p->m);
	printf("[CONSUMATORE] Lasciato il monitor...\n");
    return valore;

}

void rimuovi_vettore(VettoreProdCons * p) {

    /* TBD: De-inizializzare la struttura dati "VettoreProdCons" */
    remove_monitor(&(p->m));
   	 printf("Monitor del vettore correttamente deallocato\n");
}


void inizializza_buffer(BufferMutuaEsclusione * p) {

    /* TBD: Inizializzare la struttura dati "BufferMutuaEsclusione" */
    //Prendo la key
    key_t key_sem_buffer = ftok(".",'c');
    
    //Prendo il descrittore del buffer e lo piazzo nella variabile per la mutua esclusione
    p->sem_id = semget(key_sem_buffer,1,IPC_CREAT|0664);
    
    if (p->sem_id < 0){
    	
    	printf("Errore inizializzazione buffer\n");
    	exit(1);
    
    }
    
    int ret = semctl(p->sem_id,MUTEX,SETVAL,1);
    
    if(ret < 0) {
        perror("Errore semctl");
        exit(1);
    }
    
    //Inizializzo elementi a 0    
   	p->buffer=0;
    
    printf("Buffer correttamente inizializzato\n");
}

void aggiorna(BufferMutuaEsclusione * p, int valore) {

    /* TBD: Aggiungere la sincronizzazione per la mutua esclusione */

	//Devo semplicemente fare l'accesso in mutua esclusione sul semaforo
	Wait_Sem(p->sem_id,MUTEX);
	
		//Posso aggiornare il valore
    	p->buffer += valore;

    	printf("[AGGIORNA] Nuovo valore del buffer: %d\n", p->buffer);
    	
    //Posso uscire
    Signal_Sem(p->sem_id,MUTEX);
}

void rimuovi_buffer(BufferMutuaEsclusione * p) {

    /* TBD: De-inizializzare la struttura dati "BufferMutuaEsclusione" */
    semctl(p->sem_id,0,IPC_RMID);
    printf("Buffer correttamente deallocato\n");
}


