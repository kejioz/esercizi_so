#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>

#include "header.h"

#define NUM_MESSAGGI_DA_RICEVERE 10

void * worker(void *);

typedef struct {

    int valore;
    BufferMutuaEsclusione * buffer;

} parametri_worker;

int main() {

    /* TBD: Accedere alla coda di messaggi allocata dal processo padre */
	key_t key_coda = ftok(FTOK_PATH_CODA,FTOK_CHAR_CODA);
	int id_coda = msgget(key_coda,IPC_CREAT|0664);


    /* TBD: Allocare la struttura dati da condividere con i thread worker */
    parametri_worker params[NUM_THREADS];
    /* TBD: Completare l'inizializzazione di "buffer" */

    BufferMutuaEsclusione * buffer = (BufferMutuaEsclusione*) malloc(sizeof(BufferMutuaEsclusione));
    pthread_attr_t attr;
    pthread_mutex_init(&(buffer->mutex),NULL);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
    buffer->valore = 0;

    /* Il programma avvierà un thread worker distinto 
     * per ogni messaggio ricevuto
     */
    pthread_t thread_worker[NUM_MESSAGGI_DA_RICEVERE];

    for(int i=0; i<NUM_MESSAGGI_DA_RICEVERE; i++) {

        /* TBD: Effettuare la ricezione del messaggio */
        messaggio mess;
		msgrcv(id_coda,(void*)&mess,sizeof(messaggio)-sizeof(long),0,0);
        printf("[SERVER] Ricezione: %d\n", mess.msg);
        
        params[i].buffer = buffer;
        params[i].valore = mess.msg;


        /* TBD: Creare un thread figlio, facendogli eseguire la
         *      funzione "worker", e passandogli sia il valore ricevuto
         *      sia il "buffer", tramite la struttura dati "parametri_worker"
         */
         pthread_create(&thread_worker[i],&attr, worker, (void*) &params[i]);      
    }


    /* TBD: Attendere la terminazione dei thread figli */
	for (int t=0; t<NUM_THREADS; t++) {
		pthread_join(thread_worker[t], NULL);
		printf("[%d] Thread terminato.\n", t);
	}
	
	printf("[Server] Valore buffer finale: %d.\n", buffer->valore);

    /* TBD: De-allocazione della struttura dati "buffer" */
	free(buffer);
	pthread_mutex_destroy(&buffer->mutex);
	pthread_attr_destroy(&attr);

    return 0;

}


void * worker(void * p) {
	parametri_worker* params = (parametri_worker*) p;
    BufferMutuaEsclusione * buffer =params->buffer;/* TBD: Completare il passaggio dei parametri */;
    int valore = params->valore;/* TBD: Completare il passaggio dei parametri */;


    /* TBD: Effettuare la modifica del buffer in mutua esclusione */
	pthread_mutex_lock(&params->buffer->mutex);
    sleep(1);

    buffer->valore += valore;

    printf("[WORKER] Nuovo valore del buffer: %d\n", buffer->valore);
	pthread_mutex_unlock(&params->buffer->mutex);

    return NULL;
}
