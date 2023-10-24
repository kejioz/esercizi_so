#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "header.h"

#define NUM_PRODUTTORI 2
#define NUM_CONSUMATORI 20

#define PRODUZIONI 10

void * produttore(void *);
void * consumatore(void *);
void * visualizza(void *);

typedef struct {

    VettoreProdCons * vettore;
    BufferMutuaEsclusione * buffer;

} parametri_consumatore;

int main() {

    pthread_t produttori[NUM_PRODUTTORI];

    pthread_t consumatori[NUM_CONSUMATORI];
    
    pthread_t visualizzatore;

    VettoreProdCons * vettore = malloc(sizeof(VettoreProdCons));

    BufferMutuaEsclusione * buffer = malloc(sizeof(BufferMutuaEsclusione));


    srand(getpid());

    inizializza_vettore(vettore);

    inizializza_buffer(buffer);
    
    parametri_consumatore * parametri = malloc(sizeof(parametri_consumatore));
    parametri->vettore = vettore;
    parametri->buffer = buffer;


    for(int i=0; i<NUM_PRODUTTORI; i++) {

        pthread_create(&produttori[i], NULL, produttore, vettore);
    }

    for(int i=0; i<NUM_CONSUMATORI; i++) {

        pthread_create(&consumatori[i], NULL, consumatore, parametri);
    }
    
    pthread_create(&visualizzatore, NULL, visualizza, buffer);

    for(int i=0; i<NUM_PRODUTTORI; i++) {
        pthread_join(produttori[i], NULL);
    }

    for(int i=0; i<NUM_CONSUMATORI; i++) {
        pthread_join(consumatori[i], NULL);
    }
    
    pthread_join(visualizzatore, NULL);

    rimuovi_buffer(buffer);
    rimuovi_vettore(vettore);
    
    free(parametri);
    free(buffer);
    free(vettore);

    return 0;

}

void * produttore(void * p) {

    VettoreProdCons * vettore = p;

    for(int i=0; i<PRODUZIONI; i++) {

        int valore = rand() % 10;

        printf("[PRODUTTORE] Produzione: %d\n", valore);

        produci(vettore, valore);
    }

    return NULL;
}

void * consumatore(void * p) {

    parametri_consumatore * parametri = p;

    int valore;

    valore = consuma(parametri->vettore);

    printf("[CONSUMATORE] Consumazione: %d\n", valore);

    aggiorna_buffer(parametri->buffer, valore);

    return NULL;
}

void * visualizza(void * p) {

     for (int i=0; i< (NUM_PRODUTTORI*PRODUZIONI)/NUM_CONS; i++) {
     
             BufferMutuaEsclusione * buf = p;
             
             int val;
             
             val = stampa_valore(buf);
             
             printf("[VISUALIZZATORE]: La somma degli elementi consumati è: %d\n",val);
     
     }
     
     return NULL;

}