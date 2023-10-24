#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <time.h>

#include "header.h"

#define NUM_MESSAGGI_DA_INVIARE 5

int main() {

    /* TBD: Accedere alla coda di messaggi allocata dal processo padre */
	key_t key_coda = ftok(FTOK_PATH_CODA,FTOK_CHAR_CODA);
	int id_coda = msgget(key_coda,IPC_CREAT|0664);

    srand(getpid());
    
    for(int i=0; i<NUM_MESSAGGI_DA_INVIARE; i++) {

        int valore = rand() % 10;

        printf("[CLIENT] Invio: %d\n", valore);

        /* TBD: Inviare il valore tramite un messaggio */
        messaggio mess;
		mess.type = 1;
		mess.msg = valore;
		
		msgsnd(id_coda,(void*) &mess,sizeof(messaggio)-sizeof(long),0);
		printf("[CLIENT] Inviato correttamente %d\n", valore);
		
    }


    return 0;

}
