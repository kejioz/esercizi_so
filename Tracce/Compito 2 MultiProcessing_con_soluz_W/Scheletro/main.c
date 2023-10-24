#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include "header.h"

#define NUM_PRODUTTORI 2
#define NUM_CONSUMATORI 5

#define PRODUZIONI 10
#define CONSUMAZIONI 4

void produttore(VettoreProdCons * vettore);
void consumatore(VettoreProdCons * vettore, BufferMutuaEsclusione * buffer);

int main() {

	
    VettoreProdCons * vettore; /* TBD: Allocare la struttura dati */
    
    key_t key_vett = ftok(".",'d');
    
    int vett_id = shmget(key_vett,sizeof(VettoreProdCons),IPC_CREAT|0664);
    
    if (vett_id < 0){
    
    	 printf("Errore nell'allocazione del vettore\n");
    	 exit(1);
    
    }
    
    vettore = shmat(vett_id,0,0);
    
    if(vettore == (void*) -1) {
        perror("Errore shmat");
        exit(1);
    }

    BufferMutuaEsclusione * buffer; /* TBD: Allocare la struttura dati */

	key_t key_buf = ftok(".",'f');
	
	int buf_id = shmget(key_buf,sizeof(BufferMutuaEsclusione),IPC_CREAT|0664);
    
    if (buf_id < 0){
    
    	 printf("Errore nell'allocazione del buffer\n");
    	 exit(1);
    
    }
    
    buffer = shmat(buf_id,0,0);
    
    if(buffer == (void*) -1) {
        perror("Errore shmat");
        exit(1);
    }
	
    inizializza_vettore(vettore);
    inizializza_buffer(buffer);

	int j;
	
    for(int i=0; i<NUM_PRODUTTORI; i++) {

        /* TBD: Creare i processi produttori, e fargli chiamare la
         *      funzione "produttore()"
         */
         
        pid_t pid = fork();
        
        if (pid == 0){
        	
    		produttore(vettore);
        
        	exit(0);
        	
        }
        
    }


    for(int i=0; i<NUM_CONSUMATORI; i++) {

        /* TBD: Creare i processi consumatori, e fargli chiamare la
         *      funzione "consumatore()"
         */
         
         pid_t pid = fork();
         
         if (pid == 0){
         
         	consumatore(vettore,buffer);
         
         	exit(0);
         }
    }


    /* TBD: Attendere la terminazione dei processi figli */

	for (j=0;j<NUM_PRODUTTORI+NUM_CONSUMATORI;j++){
		wait(NULL);
	}
	
	printf("Processi terminati. Il valore del buffer è %d\n",buffer->buffer);

    rimuovi_buffer(buffer);
    rimuovi_vettore(vettore);


    /* TBD: De-allocare le strutture dati */
    shmctl(vett_id,IPC_RMID,0);
	shmctl(buf_id,IPC_RMID,0);
	printf("VETTORE E BUFFER CORRETTAMENTE DEALLOCATI. FINE. \n");
    return 0;

}

void produttore(VettoreProdCons * vettore) {

    srand(getpid());
    
    for(int i=0; i<PRODUZIONI; i++) {

        int valore = rand() % 10;

        printf("[MAIN PRODUTTORE] Produzione: %d\n", valore);

        /* TBD: Chiamare il metodo "produci()" del monitor */
        produci(vettore,valore);
    }

}

void consumatore(VettoreProdCons * vettore, BufferMutuaEsclusione * buffer) {

    srand(getpid());

    for(int i=0; i<CONSUMAZIONI; i++) {

        int valore;

        /* TBD: Chiamare il metodo "consuma()" del monitor */
        
        valore = consuma(vettore);

        printf("[MAIN CONSUMATORE] Consumazione: %d\n", valore);

        /* TBD: Chiamare il metodo "aggiorna()", passandogli 
         *      il valore che è stato appena consumato
         */
         
         aggiorna(buffer,valore);
    }
}
