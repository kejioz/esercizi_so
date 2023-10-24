#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "header.h"

typedef struct {
    int buffer[MAX_VALUES];
    // TODO aggiungere le variabili per la gestione circolare
    // e per la sincronizzazione dei thread
    
    int testa;
    int coda;
    int elems;
    
    pthread_mutex_t mutex;
    
    pthread_cond_t cv_consumatori;
    pthread_cond_t cv_produttori;

    
} data_str;

typedef struct {

    int somma;
    int num_valori_sommati;
    // TODO aggiungere le variabili per la sincronizzazione dei thread
    pthread_mutex_t mutex;
    
    pthread_cond_t cv_stampasomma;
    
} somma_valori;

// VARIABILI GLOBALI:
// la struttura
data_str shared_buf;
// le code
int queue_req;
int queue_res;

int consuma() {
    int value;
    //TODO consumatore, con gestione coda circolare
    
    //Per prima cosa devo acquisire il lock sulla struttura shared_buf
    pthread_mutex_lock(&(shared_buf.mutex));
    
    	//Sono dentro. Check sulla variabile condition. Ricorda while
    	while ( shared_buf.elems == 0 ){
    		
    		//Se non ci sono elementi non posso consumare. Quindi devo fare una wait 
    		pthread_cond_wait(&(shared_buf.cv_consumatori),&(shared_buf.mutex));
    	}
    	
    	//A questo punto posso consumare, decrementare il numero di elementi, aumentare punt
    	value = shared_buf.buffer[shared_buf.testa];
    	shared_buf.elems = (shared_buf.elems -1) % MAX_VALUES;
    	shared_buf.testa = (shared_buf.testa +1) % MAX_VALUES;
    	
    	//A questo punto posso fare la signal sulla condvar pieno
    	pthread_cond_signal(&(shared_buf.cv_produttori));
    	
    pthread_mutex_unlock(&(shared_buf.mutex));
    
    printf("[CONSUMATORE] Ho consumato con successo %d\n",value);

    return value;
}

void produci(int new_value) {

    //TODO produttore, con gestione coda circolare
    
    //Acquisisco il lock
    pthread_mutex_lock(&(shared_buf.mutex));
    
    	//Sono dentro. Check sulla condvar. Ricorda while
    	while ( shared_buf.elems == MAX_VALUES){
    		
    		//Mi devo sospendere finchè non si libera spazio
    		pthread_cond_signal(&(shared_buf.cv_produttori));
    	
    	}
    	
    	//A questo punto posso produrre, aumentare elems e coda. Poi segnalo i consumatori
    	shared_buf.buffer[shared_buf.coda] = new_value;
    	shared_buf.elems = (shared_buf.elems+1) % MAX_VALUES;
    	shared_buf.coda = (shared_buf.coda+1) % MAX_VALUES;
    	
    	//Segnalo i consumatori
    	pthread_cond_signal(&(shared_buf.cv_consumatori));
    	
    //Posso uscire 
    pthread_mutex_unlock(&(shared_buf.mutex));
    
    printf("[PRODUTTORE] Ho prodotto con successo %d\n",new_value);
    	
}


void * stampa_somma(void* par) {
    
    somma_valori * s = (somma_valori*) par/*TODO: recupero parametro*/;
    
    //TODO attesa che siano stati sommati NUM_CONS valori utilizzando una condition
    // e stampa della somma una volta verificata la condizione
    
    //Per prima cosa acquiscisco il lock
    pthread_mutex_lock(&s->mutex);
    
    	//Aspetto fintantochè non sono stati sommati 12 valori
    	while(s->num_valori_sommati < 12){
    		pthread_cond_wait(&s->cv_stampasomma,&s->mutex);
    	}
    	
    	printf("[THREADSTAMPASOMMA] FINALMENTE SONO STATI SOMMATI 12 VALORI! IL VALORE DEL BUFFER E' %d!\n",s->somma);
    	
    //Lascio il lock
    pthread_mutex_unlock(&s->mutex);

    pthread_exit(NULL);
}

void * produttore(void* par) {
    // par qui non serve, può essere ignorato
    int new_value;
    int i;
    for (i = 0; i < NUM_UPDATES*MAX_VALUES; i++) {
        new_value = rand()%10+1;
        printf("PRODUTTORE: inserimento nuovo dato: %d\n",new_value);
        produci(new_value);
        sleep(rand()%3+1);
    }
    pthread_exit(NULL);
}


void * consumatore(void* par) {
		
	//Linko il puntatore alla struttura di somma
    somma_valori * s = (somma_valori*) par;
    int i;
    for (i = 0; i < NUM_CONS; i++) {
        req msg;
        //TODO ricezione messaggio di richiesta
        printf("CONSUMATORE_SERV: ricevuta richiesta di consumo\n");
        msgrcv(queue_req,&msg,sizeof(msg)-sizeof(long),0,0);

        res risp;
        //TODO preparazione messaggio di risposta, usando funzione "consuma"
        int valore = consuma();
        risp.type = TIPOMESS;
        risp.value = valore;
        printf("CONSUMATORE_SERV: invio valore al consumatore client %d\n",risp.value);
        //TODO invio risposta
        msgsnd(queue_res,&risp,sizeof(msg)-sizeof(long),0);
        
        //TODO aggiornamento della somma in 's' con il nuovo valore appena consumato
        
        pthread_mutex_lock(&s->mutex);
        
        	//Sono dentro al buffer. Posso schiattare la somma
        	s->somma = s->somma + valore;
        	//Aggiorno il numero di valori sommati
        	s->num_valori_sommati = s->num_valori_sommati + 1;
        	
        //TODO e notifica della condizione quando sommati NUM_CONS elementi
        
        	//Se sono dodici posso fare la signal sulla condvar
        	if (s->num_valori_sommati == 12){
        		pthread_cond_signal(&s->cv_stampasomma);
        		printf("12 VALORI SOMMATI CORRETTAMENTE! POSSO NOTIFICARE LA STAMPA SOMMA!");
        	}
        	
        //Lascio il lock
        pthread_mutex_unlock(&s->mutex);
        
    }

    pthread_exit(NULL);
}

int main() {

    //TODO: completare l'inizializzazione code
    key_t msg_req_key = ftok(FTOK_PATH,FTOK_CHAR_RICHIESTE);
	key_t msg_res_key = ftok(FTOK_PATH,FTOK_CHAR_RISPOSTE);

    queue_req = msgget(msg_req_key,IPC_CREAT|0664);
	queue_res = msgget(msg_res_key,IPC_CREAT|0664);

    //TODO inizializzazione di "shared_buf"

	shared_buf.elems = 0;
	shared_buf.testa = 0;
	shared_buf.coda = 0;

    pthread_t prod, cons, sum;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    srand(time(NULL));

	/*TODO allocazione buffer*/
    somma_valori* s = (somma_valori*) malloc(sizeof(somma_valori));
    
    // TODO e inizializzazione valori
    s->somma=0;
    s->num_valori_sommati=0;
    pthread_mutex_init(&s->mutex,NULL);
    pthread_cond_init(&s->cv_stampasomma,NULL);
    
    //TODO creazione dei tre thread prod cons e sum
    // nota: ai thread cons e sum bisogna passare la struttura s
    
    //Produttore
	pthread_create(&prod,&attr,(void*) produttore, (void*)NULL);
	pthread_create(&cons,&attr,(void*) consumatore, (void*) s);
	pthread_create(&sum,&attr,(void*) stampa_somma, (void*) s);
    //TODO join
    
 	pthread_join(prod,NULL);
 	printf("Thread prod terminato\n");
 	pthread_join(cons,NULL);
 	printf("Thread cons terminato\n");
 	pthread_join(sum,NULL);
 	printf("Thread sum terminato\n");
 	

    //TODO rimozione code e struttura s
    msgctl(queue_req,IPC_RMID,0);
	msgctl(queue_res,IPC_RMID,0);
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&s->mutex);
	pthread_cond_destroy(&s->cv_stampasomma);
	free(s);
	printf("[SERVER] : rimosse correttamente le code e la struttura s\n");

    return 0;
}
