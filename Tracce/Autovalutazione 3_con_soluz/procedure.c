#include <stdio.h>
#include <unistd.h>

#include "header.h"

void inizializza_vettore(VettoreProdCons * p) {

    pthread_mutex_init(&p->mutex, NULL);
    pthread_cond_init(&p->cv_prod, NULL);
    pthread_cond_init(&p->cv_cons, NULL);

    p->num_liberi = DIM;
    p->num_occupati = 0;

    for(int i=0; i<DIM; i++) {
        p->stato[i] = LIBERO;
    }
}

void produci(VettoreProdCons * p, int valore) {

    int indice = 0;

    pthread_mutex_lock(&p->mutex);

    while(p->num_liberi == 0) {
        pthread_cond_wait(&p->cv_prod, &p->mutex);
    }


    while(indice<DIM && p->stato[indice] != LIBERO) {
        indice++;
    }

    p->stato[indice] = INUSO;
    p->num_liberi--;

    pthread_mutex_unlock(&p->mutex);


    p->buffer[indice] = valore;
    sleep(1);


    pthread_mutex_lock(&p->mutex);

    p->num_occupati++;
    p->stato[indice] = OCCUPATO;

    pthread_cond_signal(&p->cv_cons);

    pthread_mutex_unlock(&p->mutex);
}

int consuma(VettoreProdCons * p) {

    int valore;

    int indice = 0;

    pthread_mutex_lock(&p->mutex);

    while(p->num_occupati == 0) {
        pthread_cond_wait(&p->cv_cons, &p->mutex);
    }

    while(indice<DIM && p->stato[indice] != OCCUPATO) {
        indice++;
    }

    p->stato[indice] = INUSO;
    p->num_occupati--;

    pthread_mutex_unlock(&p->mutex);


    valore = p->buffer[indice];
    sleep(1);


    pthread_mutex_lock(&p->mutex);

    p->num_liberi++;
    p->stato[indice] = LIBERO;

    pthread_cond_signal(&p->cv_prod);

    pthread_mutex_unlock(&p->mutex);

    return valore;

}

void rimuovi_vettore(VettoreProdCons * p) {

    pthread_mutex_destroy(&p->mutex);
    pthread_cond_destroy(&p->cv_prod);
    pthread_cond_destroy(&p->cv_cons);
}


void inizializza_buffer(BufferMutuaEsclusione * p) {

    pthread_mutex_init(&p->mutex, NULL);
    pthread_cond_init(&p->cv_somme, NULL);
    p->buffer = 0;
    p->num_somme = 0;
}

void aggiorna_buffer(BufferMutuaEsclusione * p, int valore) {

    pthread_mutex_lock(&p->mutex);
    
    /*
    
        NOTA: soluzione alternativa: se num_somme è uguale a NUM_CONS sospendo l'aggiornamento del buffer
    
    */

    p->buffer += valore;
    
    p->num_somme++;
    
    if (p->num_somme == NUM_CONS)
            pthread_cond_signal(&p->cv_somme);

    /*
    
        NOTA: soluzione alternativa: [...] else sblocca un consumatore
    
    */

    pthread_mutex_unlock(&p->mutex);
}

int stampa_valore(BufferMutuaEsclusione * p) {

    int val;

    pthread_mutex_lock(&p->mutex);
    
    while (p->num_somme < NUM_CONS ) {
        pthread_cond_wait(&p->cv_somme,&p->mutex);
    }
    
    val = p->buffer;
    p->num_somme = 0;
    
    /*
    
        NOTA: soluzione alternativa: risveglia un consumatore
    
    */
        
    pthread_mutex_unlock(&p->mutex);

    return val;
}

void rimuovi_buffer(BufferMutuaEsclusione * p) {

    pthread_mutex_destroy(&p->mutex);
    pthread_cond_destroy(&p->cv_somme);
}
