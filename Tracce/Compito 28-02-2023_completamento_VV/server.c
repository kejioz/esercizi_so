#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>

#include "header_prodcons.h"
#include "header_msg.h"

void server(int id_queue_server) {

    BufferProdCons * b = (BufferProdCons*) malloc(sizeof(BufferProdCons));/* TBD: Creare e inizializzare l'oggetto monitor */

	b->stato = VUOTO;
	pthread_mutex_init(&(b->mutex),NULL);
	pthread_cond_init(&(b->cv_consumatori),NULL);
	pthread_cond_init(&(b->cv_produttori),NULL);
	
	printf("[SERVER] Inizializzato il buffer a vuoto, il mutex e le cv correttamente.\n");

    /* TBD: Creare un thread, 
            e fargli eseguire la funzione "visualizza()",
            passandogli in ingresso l'oggetto monitor
     */
     
     pthread_t thread_visualizzatore;
     
     pthread_create(&thread_visualizzatore,NULL,visualizza,(void*) b);


    for(int i=0; i<NUM_MESSAGGI_PER_SENSORE; i++) {

        int somma_valori = 0;
        int media = 0;

        for(int j=0; j<NUM_COLLETTORI; j++) {

            int id_collettore = j+1;

            messaggio_collettore m;

            /* TBD: Ricevere un messaggio da un collettore, con ricezione selettiva */
            msgrcv(id_queue_server,&m,sizeof(messaggio_collettore)-sizeof(long),id_collettore,0);

            int valore = m.valore;

            printf("[SERVER] Ricevuto valore=%d\n", valore);

            somma_valori += valore;

        }

        media = somma_valori / NUM_COLLETTORI;

        /* TBD: Chiamare la funzione "produci_valore()",
                passandogli la variabile "media"
        */
        produci_valore(b,media);

    }


    /* TBD: Attendere la terminazione del thread, de-allocare il monitor */
    pthread_join(thread_visualizzatore,NULL);
    
    pthread_mutex_destroy(&(b->mutex));
	pthread_cond_destroy(&(b->cv_consumatori));
	pthread_cond_destroy(&(b->cv_produttori));

}

void * visualizza(void * p) {

	p = (BufferProdCons*) p;

    for(int i=0; i<NUM_MESSAGGI_PER_SENSORE; i++) {

        int valore = consuma_valore(p);/* TBD: Utilizzare la funzione "consuma_valore()" */

        printf("[VISUALIZZA] Valore=%d\n", valore);
    }

    return NULL;
}


void produci_valore(BufferProdCons * b, int valore) {

    /* TBD: Completare questa funzione introducendo la sincronizzazione */

	//Check sulla condvar. Ricorda while
	
	while(b->stato == PIENO){
		pthread_cond_wait(&(b->cv_produttori),&(b->mutex));
	}
	
    	b->valore = valore;
    	b->stato = PIENO;
    	
    	printf(" ----- PRODUTTORE ----- PRODOTTO CORRETTAMENTE %d \n",b->valore);
    	
    pthread_cond_signal(&(b->cv_consumatori));
    
    printf(" ----- PRODUTTORE ----- SEGNALATO CONSUMATORI \n");

}

int consuma_valore(BufferProdCons * b) {

    /* TBD: Completare questa funzione introducendo la sincronizzazione */

	while(b->stato == VUOTO){
		pthread_cond_wait(&(b->cv_consumatori),&(b->mutex));
	}
	
    	int valore = b->valore;
	
    	b->stato = VUOTO;
    	
    	printf(" ----- CONSUMATORE ----- CONSUMATO CORRETTAMENTE %d \n",b->valore);
    	
    pthread_cond_signal(&(b->cv_produttori));
    
    printf(" ----- CONSUMATORE----- SEGNALATO PRODUTTORE  \n");

    return valore;
    
}
