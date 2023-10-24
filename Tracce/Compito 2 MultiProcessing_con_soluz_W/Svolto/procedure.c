#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "header.h"

void inizializza_vettore(VettoreProdCons * p) {

    p->testa = 0;
    p->coda = 0;

    init_monitor(&p->m, 2);
}

void produci(VettoreProdCons * p, int valore) {

    enter_monitor(&p->m);

    if( ((p->testa + 1) % DIM) == p->coda ) {
        wait_condition(&p->m, CV_PROD);
    }


    p->buffer[p->testa] = valore;
    p->testa = (p->testa + 1) % DIM;


    signal_condition(&p->m, CV_CONS);

    leave_monitor(&p->m);
}

int consuma(VettoreProdCons * p) {

    int valore;

    enter_monitor(&p->m);

    if( p->testa == p->coda ) {
        wait_condition(&p->m, CV_CONS);
    }


    valore = p->buffer[p->coda];
    p->coda = (p->coda + 1) % DIM;


    signal_condition(&p->m, CV_PROD);

    leave_monitor(&p->m);

    return valore;

}

void rimuovi_vettore(VettoreProdCons * p) {

    remove_monitor(&p->m);
}


void inizializza_buffer(BufferMutuaEsclusione * p) {

    key_t chiave_semaforo = ftok(".", 'c');

    int id_semaforo = semget(chiave_semaforo, 1, IPC_CREAT|0664);

    if(id_semaforo < 0) {
        perror("Errore semget");
        exit(1);
    }


    int ret = semctl(id_semaforo, MUTEX, SETVAL, 1);

    if(ret < 0) {
        perror("Errore semctl");
        exit(1);
    }

    p->sem_id = id_semaforo;

    p->buffer = 0;
}

void aggiorna(BufferMutuaEsclusione * p, int valore) {

    Wait_Sem(p->sem_id, MUTEX);

    p->buffer += valore;

    printf("[AGGIORNA] Nuovo valore del buffer: %d\n", p->buffer);

    Signal_Sem(p->sem_id, MUTEX);
}

void rimuovi_buffer(BufferMutuaEsclusione * p) {

    semctl(p->sem_id, 0, IPC_RMID);
}


