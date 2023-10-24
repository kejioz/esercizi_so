#include "header.h"



void inizializza(struct monitor* m){

	m->molo=0;
	m->id_nave=0;

    /* TODO: Inizializzare le variabili dell'algoritmo, il mutex, e le variabili condition */
    m->numero_lettori = 0;
    m->numero_scrittori =0;
    m->coda_scrittori =0;
    m->coda_lettori=0;
    
    //Inizializzo il mutex
    pthread_mutex_init(&(m->mutex),NULL);
    
    //Inizializzo le cv
    pthread_cond_init(&(m->cv_lettori),NULL);
    pthread_cond_init(&(m->cv_scrittori),NULL);

}

void rimuovi (struct monitor* m){
    /* TODO: Disattivare mutex e variabili condition */
    pthread_cond_destroy(&(m->cv_scrittori));
    pthread_cond_destroy(&(m->cv_lettori));
    pthread_mutex_destroy(&(m->mutex));
}


//SCRITTURA. AGGIORNAMENTO DELLA POSIZIONE DEL TRENO
void scrivi_molo(struct monitor* m, int molo){

    /* TODO: Implementare qui lo schema dei lettori-scrittori con starvation di entrambi.
     * nella parte della SCRITTURA
     */
    
    //Come prima cosa acquisisco il lcok
    pthread_mutex_lock(&(m->mutex));
    
    	//Una volta che sono dentro devo fare il check sulla condition variable
    	while (m->numero_lettori >0 || m->numero_scrittori >0){
    		
    		m->coda_scrittori++;
    		//Se ci sono già lettori o scrittori, devo fare la wait sulla condition variable
    		pthread_cond_wait(&(m->cv_scrittori),&(m->mutex));
    		m->coda_scrittori--;
    	}
    	
    	//Se sono qui vuol dire che sono potuto entrare. Posso scrivere il molo
    	m->numero_scrittori++;
    	
    //Posso rilasciare il lock
    pthread_mutex_unlock(&(m->mutex));
    
    //Posso produrre
    m->molo = molo;
    printf("[SCRITTORE] Ho prodotto %d\n",molo);
    
    //Rientro
    pthread_mutex_lock(&(m->mutex));
    
    	//Posso decrementare il numero di scrittori siccome ho finito
    	m->numero_scrittori--;
    	
    	//A questo punto se ci sono scrittori in attesa segnalo loro, altrimenti faccio broadcast a tutti i lettori
    	if ( m->coda_scrittori >0){
    	
    		pthread_cond_signal(&(m->cv_scrittori));
    		printf("[SCRITTORE] Ho segnalato un altro scrittore che può entrare a scrivere\n");
    		
    	}
    	else {
    	
    		pthread_cond_broadcast(&(m->cv_lettori));
    		printf("[SCRITTORE] Ho segnalato tutti i lettori che possono leggere\n");
    		
    	}
    	
    //Rilascio il lock
    pthread_mutex_unlock(&(m->mutex));
}


//LETTURA. RESTITUISCE LA POSIZIONE DEL TRENO
int leggi_molo(struct monitor* m){

	int valore;
    /* TODO: Implementare qui lo schema dei lettori-scrittori con starvation di entrambi.
     * nella parte della LETTURA
     */
     
     //Per prima cosa devo acquisire il lock sull'oggetto
     pthread_mutex_lock(&(m->mutex));
     
     	//Check sulla condition variable
     	while (m->numero_scrittori>0){
     		m->coda_lettori++;
     		pthread_cond_wait(&(m->cv_lettori),&(m->mutex));
     		m->coda_lettori--;
     	}
     	
     	m->numero_lettori++;
     
     //A QUESTO PUNTO DEVO RILASCIARE IL LOCK! IMPORTANTE!
     pthread_mutex_unlock(&(m->mutex));
     
     //Leggo. Posso farlo perchè ho aumentato il numero di lettori
     valore = m->molo;
     printf("[LETTORE] Ho letto %d\n",valore);
     
     //A questo punto riacquisico il lock per modificare il numero di lettori
     pthread_mutex_lock(&(m->mutex));
     
     	m->numero_lettori--;
     	
     	//Se sono l'ultimo lettore, segnalo gli scrittori
     	if (m->numero_lettori == 0){
     		pthread_cond_signal(&(m->cv_scrittori));
     		printf("[LETTORE] Ho segnalato tutti gli scrittori che possono scrivere\n");
     	}
     	
     //Lascio il lock
     pthread_mutex_unlock(&(m->mutex));

}
