#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#include "header.h"

#define NUM_PRODUTTORI 2
#define NUM_CONSUMATORI 5

#define PRODUZIONI 10
#define CONSUMAZIONI 4

void produttore(VettoreProdCons * vettore);
void consumatore(VettoreProdCons * vettore, BufferMutuaEsclusione * buffer);

int main() {

    key_t chiave_vettore = ftok(".", 'a');

    int id_vettore = shmget(chiave_vettore, sizeof(VettoreProdCons), IPC_CREAT|0664);

    if(id_vettore < 0) {
        perror("Errore shmget");
        exit(1);
    }

    key_t chiave_buffer = ftok(".", 'b');

    int id_buffer = shmget(chiave_buffer, sizeof(BufferMutuaEsclusione), IPC_CREAT|0664);

    if(id_buffer < 0) {
        perror("Errore shmget");
        exit(1);
    }



    VettoreProdCons * vettore = shmat(id_vettore, NULL, 0);

    if(vettore == (void*) -1) {
        perror("Errore shmat");
        exit(1);
    }


    BufferMutuaEsclusione * buffer = shmat(id_buffer, NULL, 0);

    if(vettore == (void*) -1) {
        perror("Errore shmat");
        exit(1);
    }


    inizializza_vettore(vettore);
    inizializza_buffer(buffer);


    for(int i=0; i<NUM_PRODUTTORI; i++) {

        pid_t pid = fork();

        if(pid == 0) {

            produttore(vettore);

            exit(0);
        }
    }


    for(int i=0; i<NUM_CONSUMATORI; i++) {

        pid_t pid = fork();

        if(pid == 0) {

            consumatore(vettore, buffer);

            exit(0);
        }
    }


    for(int i=0; i<NUM_PRODUTTORI; i++) {
        wait(NULL);
    }

    for(int i=0; i<NUM_CONSUMATORI; i++) {
        wait(NULL);
    }


    rimuovi_buffer(buffer);
    rimuovi_vettore(vettore);


    shmctl(id_vettore, IPC_RMID, NULL);
    shmctl(id_buffer, IPC_RMID, NULL);

    return 0;

}

void produttore(VettoreProdCons * vettore) {

    srand(getpid());
    
    for(int i=0; i<PRODUZIONI; i++) {

        int valore = rand() % 10;

        printf("[MAIN PRODUTTORE] Produzione: %d\n", valore);

        produci(vettore, valore);
    }

}

void consumatore(VettoreProdCons * vettore, BufferMutuaEsclusione * buffer) {

    srand(getpid());

    for(int i=0; i<CONSUMAZIONI; i++) {

        int valore;

        valore = consuma(vettore);

        printf("[MAIN CONSUMATORE] Consumazione: %d\n", valore);

        aggiorna(buffer, valore);
    }
}
