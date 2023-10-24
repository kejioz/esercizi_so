#include <stdio.h>
#include <stdlib.h>

#include "prodcons.h"
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define NUM_THREAD_PROD 3
#define NUM_THREAD_CONS 3
#define NUM_PRODUZIONI 10
#define NUM_CONSUMAZIONI 10

void * thread_produttore(void *);
void * thread_consumatore(void *);
void * thread_conteggio(void *);


int main() {

    /* TBD: Creare un oggetto monitor di tipo "MonitorProdCons" */
    MonitorProdCons* m = (MonitorProdCons*) malloc(sizeof(MonitorProdCons));
    
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
    
    pthread_t produttori[NUM_THREAD_PROD];
    pthread_t consumatori[NUM_THREAD_CONS];
    pthread_t conteggio;

    init_monitor(m);

    for(int i=0; i<NUM_THREAD_PROD; i++) {

        /* TBD: Creare un thread produttore, fargli eseguire la funzione "thread_produttore" */
        pthread_create(&produttori[i],&attr,thread_produttore,(void*) m);
        sleep(1);
        printf(" --- Sistema --- Avviato produttore numero %d\n",i+1);
        
    }

    for(int i=0; i<NUM_THREAD_CONS; i++) {

        /* TBD: Creare un thread consumatore, fargli eseguire la funzione "thread_consumatore" */
        pthread_create(&consumatori[i],&attr,thread_consumatore,(void*) m);
        sleep(1);
        printf(" --- Sistema --- Avviato consumatore numero %d\n",i+1);
    }


    /* TBD: Creare un thread di "conteggio", fargli eseguire la funzione "thread_conteggio" */
    pthread_create(&conteggio,&attr,thread_conteggio,(void*) m);
    printf(" --- Sistema --- Avviato il thread di conteggio\n");

    /* TBD: Attendere la terminazione dei thread */
    for(int i=0; i<NUM_THREAD_PROD; i++) {
      	pthread_join(produttori[i],NULL);
      	printf(" --- Sistema --- Terminato produttore numero %d\n",i+1);
      	
    }
    
    printf("////////////TUTTI I PRODUTTORI HANNO TERMINATO////////////.\n");
    
    for(int i=0; i<NUM_THREAD_CONS; i++) {
      	pthread_join(consumatori[i],NULL);
      	printf(" --- Sistema --- Terminato consumatore numero %d\n",i+1);
      	
    }
    
    printf("////////////TUTTI I CONSUMATORI HANNO TERMINATO////////////.\n");
    
    pthread_join(conteggio,NULL);
    printf(" --- Sistema --- Terminato conteggio.\n");

	pthread_attr_destroy(&attr);
    remove_monitor(m);

    /* TBD: Deallocare l'oggetto monitor */
    free(m);
    
    return 0;
}

void * thread_produttore(void * x) {

    /* TBD: Completare il passaggio dell'oggetto "MonitorProdCons" */
    
    MonitorProdCons* m = (MonitorProdCons*) x;

    for(int i=0; i<NUM_PRODUZIONI; i++) {

        /* TBD: Completare la chiamata a produzione() */
		sleep(1);
        int val = rand() % 10;
        produzione(m, val);

        printf("[PRODUTTORE] Ho prodotto: %d\n", val);

    }
    
	printf("???????PRexit\n");
    pthread_exit(NULL);
    printf("???????POstexit\n");
}

void * thread_consumatore(void * x) {

    /* TBD: Completare il passaggio dell'oggetto "MonitorProdCons" */
    
    MonitorProdCons* m = (MonitorProdCons*) x;

    for(int i=0; i<NUM_CONSUMAZIONI; i++) {

        /* TBD: Completare la chiamata a consumazione() */
		sleep(1);
        int val = consumazione(m);

        printf("[CONSUMATORE] Ho consumato: %d\n", val);
    }
    
    pthread_exit(NULL);
    
}


void * thread_conteggio(void * x) {

    /* TBD: Completare il passaggio dell'oggetto "MonitorProdCons" */
    
    MonitorProdCons* m = (MonitorProdCons*) x;

    int consumazioni_effettuate = 0;

    int consumazioni_da_effettuare = NUM_CONSUMAZIONI * NUM_THREAD_CONS;

    while(consumazioni_da_effettuare > 0) {

        int conteggio;

        if( consumazioni_da_effettuare >= CONTEGGIO ) {

            // Vi sono 5 o più consumazioni ancora da effettuare,
            // si pone a 5 il conteggio da attendere
            conteggio = CONTEGGIO;
        }
        else {

            // Vi sono meno di 5 consumazioni ancora da effettuare,
            // si pone il conteggio da attendere a "consumazioni_da_effettuare"
            conteggio = consumazioni_da_effettuare;
        }

        /* TBD: Completare la chiamata ad "attendi_consumazioni()" */

        int n = attendi_consumazioni(m, conteggio);

        consumazioni_effettuate += n;

        consumazioni_da_effettuare -= n;

        printf("[CONTEGGIO] Sono stati consumati in totale %d elementi\n", consumazioni_effettuate);
    }
	
    pthread_exit(NULL);

}
