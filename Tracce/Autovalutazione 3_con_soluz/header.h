#ifndef _HEADER_
#define _HEADER_

#include <pthread.h>

#define DIM 3

#define NUM_CONS 5

#define LIBERO 0
#define INUSO 1
#define OCCUPATO 2

typedef struct {

    int buffer[DIM];

    int stato[DIM];
    int num_liberi;
    int num_occupati;

    pthread_mutex_t mutex;
    pthread_cond_t cv_prod;
    pthread_cond_t cv_cons;

} VettoreProdCons;

typedef struct {

    int buffer;
    int num_somme;

    pthread_mutex_t mutex;
    pthread_cond_t cv_somme;
    
    /*
        NOTA: in questa soluzione, viene utilizzata una sola variabile per rispettare i requisiti della traccia.
              in alternativa, si può risolvere il problema utilizzando un'ulteriore variabile per sospendere i consumatori
              finché sono state effettuate 5 somme ma non sono ancora state visualizzate. 
              se il numero di somme effettuate non è uguale a NUM_CONS viene risvegliato un consumatore.
    
    */

} BufferMutuaEsclusione;

void inizializza_vettore(VettoreProdCons * p);
void produci(VettoreProdCons * p, int valore);
int consuma(VettoreProdCons * p);
void rimuovi_vettore(VettoreProdCons * p);

void inizializza_buffer(BufferMutuaEsclusione * p);
void aggiorna_buffer(BufferMutuaEsclusione * p, int valore);
void rimuovi_buffer(BufferMutuaEsclusione * p);

int stampa_valore(BufferMutuaEsclusione * p);


#endif
