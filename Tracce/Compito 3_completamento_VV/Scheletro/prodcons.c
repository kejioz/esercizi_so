#include <stdio.h>

#include "prodcons.h"


void init_monitor(MonitorProdCons * p) {

    /* TBD: Inizializzare l'oggetto monitor */

    p->conteggio_consumazioni = 0;
    p->testa=0;
    p->coda=0;
    p->elems=0;
    
    //Inizializzo le condvar e il mutex
    pthread_mutex_init(&(p->mutex),NULL);
    pthread_cond_init(&(p->cv_consumatori),NULL);
    pthread_cond_init(&(p->cv_produttori),NULL);
    pthread_cond_init(&(p->cv_soglia),NULL);
    
    printf(" --- Sistema --- Monitor inizializzato correttamente.\n");
}

void produzione(MonitorProdCons * p, int val) {

    /* TBD: Completare la sincronizzazione, in base allo schema
            del vettore di buffer circolare */
            
    //Acquisizione lock
    pthread_mutex_lock(&(p->mutex));
    
    	//Check condvar
    	while (p->elems == DIM){
    		
    		pthread_cond_wait(&(p->cv_produttori),&(p->mutex));
    		
    	}
    	
    	//Una volta qui posso produrre e segnalare ai consumatori
    	p->buffer[p->coda] = val;
    	p->coda= (p->coda +1) % DIM;
    	p->elems = p->elems +1;
    	
    	pthread_cond_signal(&(p->cv_consumatori));
    	printf(" [PRODUTTORE] Prodotto %d correttamente.\n",val);
    	
    //Relase lock
    pthread_mutex_unlock(&(p->mutex));

}

int consumazione(MonitorProdCons * p) {

    int val;

    /* TBD: Completare la sincronizzazione, in base allo schema
            del vettore di buffer circolare */

    //Acquisizione lock
    pthread_mutex_lock(&(p->mutex));
    
    	//Check condvar
    	while (p->elems == 0){
    		
    		pthread_cond_wait(&(p->cv_consumatori),&(p->mutex));
    		
    	}
    	
    	//Una volta qui posso consumare e segnalare ai produttori
    	val=p->buffer[p->testa];
    	p->testa= (p->testa +1) % DIM;
    	p->elems = p->elems -1;
    	p->conteggio_consumazioni++;
    	
    	if (p->conteggio_consumazioni == CONTEGGIO){
    	
    		pthread_cond_signal(&(p->cv_soglia));
    	
    	}
    	
    	pthread_cond_signal(&(p->cv_produttori));
    	printf(" [CONSUMATORE] Consumato %d correttamente. Consumazione numero %d.\n",val,p->conteggio_consumazioni);
    	
    //Relase lock
    pthread_mutex_unlock(&(p->mutex));
             
    return val;
}

int attendi_consumazioni(MonitorProdCons * p, int soglia) {

    int n;

    /* TBD: Sospendere il thread finché il "conteggio_consumazioni" non sia diventato
            maggiore o uguale a "soglia"
     */
     
    //Acquisizione lock
    pthread_mutex_lock(&(p->mutex));
    
    	//Check condvar
    	while (p->conteggio_consumazioni < soglia){
    		
    		pthread_cond_wait(&(p->cv_soglia),&(p->mutex));
    		
    	}    

    n = p->conteggio_consumazioni;
    
    printf(" [CONTEGGIO] Mi sono risvegliato. Effettuate %d consumazioni. Azzero tutto.\n",n);

    p->conteggio_consumazioni = 0;
    
    return n;
}

void remove_monitor(MonitorProdCons * p) {

    /* TBD: Disattivare le variabili per la sincronizzazione */
    pthread_mutex_destroy(&(p->mutex));
    pthread_cond_destroy(&(p->cv_consumatori));
    pthread_cond_destroy(&(p->cv_produttori));
    pthread_cond_destroy(&(p->cv_soglia));
    
    printf(" --- Sistema --- Monitor rimosso correttamente.\n");
}
