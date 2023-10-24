#ifndef HEADER_H
#define HEADER_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>


struct monitor {

	int molo;
	int id_nave;

	/* TODO: Aggiungere variabili per la sincronizzazione */
	//Variabili di supporto per lettore scrittore
	int numero_lettori;
	int numero_scrittori;
	
	int coda_lettori;
	int coda_scrittori;
	
	//Mutex del pthread
	pthread_mutex_t mutex;
	
	//Cv del monitor
	pthread_cond_t cv_lettori;
	pthread_cond_t cv_scrittori;
    
};

void inizializza(struct monitor * m);
void rimuovi (struct monitor * m);
void scrivi_molo(struct monitor * m, int molo);
int leggi_molo(struct monitor * m);


#endif
