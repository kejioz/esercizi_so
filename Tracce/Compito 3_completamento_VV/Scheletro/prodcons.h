#ifndef _PRODCONS_
#define _PRODCONS_

#include <pthread.h>

#define DIM 5
#define CONTEGGIO 30

typedef struct {
	
	//Buffer circolAre
    int buffer[DIM];
    int testa;
    int coda;
    int elems;

    int conteggio_consumazioni;    /* conteggio consumazioni effettuate */

    /* TBD: Aggiungere ulteriori variabili per la 
            gestione del vettore di buffer circolare
            e per la sincronizzazione
     */
     
     //Mutex pthread
     pthread_mutex_t mutex;
     
     //Condition variables 
     pthread_cond_t cv_produttori;
     pthread_cond_t cv_consumatori;
     pthread_cond_t cv_soglia;

} MonitorProdCons;


void init_monitor(MonitorProdCons * p);
void produzione(MonitorProdCons * p, int val);
int consumazione(MonitorProdCons * p);
int attendi_consumazioni(MonitorProdCons * p, int soglia);
void remove_monitor(MonitorProdCons * p);

#endif
