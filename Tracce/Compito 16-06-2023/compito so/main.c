#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <time.h>

#include "prodcons.h"

void Produttore(MonitorBufferSingolo* b, int id_coda);
void Consumatore(MonitorBufferSingolo* b, int id_coda);

int main() {

	int i;
	pid_t pid;

    /* TBD: Allocare l'oggetto MonitorBufferSingolo in memoria condivisa */;
	int id_monitor_buf_singolo = shmget(IPC_PRIVATE,sizeof(MonitorBufferSingolo),IPC_CREAT|0664);
	
		//Check
		if (id_monitor_buf_singolo < 0){
			printf("Errore\n");
			exit(1);
		}
    /* TBD: Inizializzare l'oggetto MonitorBufferSingolo */
	MonitorBufferSingolo* p = shmat(id_monitor_buf_singolo,0,0);
	
		//Check
		if (p == (void*) -1){
			printf("Errore\n");
			exit(1);
		}
		
	init_monitor_buffer_singolo(p);

    /* TBD: Allocare una coda di messaggi */
	int id_coda = msgget(IPC_PRIVATE,IPC_CREAT|0664);


    /* TBD: Creare un processo figlio, che esegua la funzione "Produttore()" */
	pid = fork();
	
	if (pid == 0){
		srand(time(NULL)*getpid());
		Produttore(p,id_coda);
		exit(0);
	}

    /* TBD: Creare un processo figlio, che esegua la funzione "Consumatore()" */
	pid = fork();
	
	if (pid == 0){
		
		Consumatore(p,id_coda);
		exit(0);
	}

    /* TBD: Attendere la terminazione dei processi figli */
	for (i=0;i<2;i++){
		wait(NULL);
		printf("Figlio %d terminato\n",i+1);
	}

    /* TBD: Deallocare l'oggetto monitor */
	remove_monitor_buffer_singolo(p);

    /* TBD: Deallocare la coda di messaggi */
    msgctl(id_coda,IPC_RMID,0);
}


void Produttore(MonitorBufferSingolo* b, int id_coda) {

    int numero_elementi = rand() % 14;

    printf("[PRODUTTORE] Totale elementi da inviare: %d\n", numero_elementi);

    /* TBD: Effettuare una produzione su MonitorBufferSingolo con
            la funzione "produzione_buffer_singolo", inserendo "numero_elementi"
     */
     produzione_buffer_singolo(b,numero_elementi);

    for(int i=0; i<numero_elementi; i++) {
    
        sleep(1);

        int val = rand() % 10;

        printf("[PRODUTTORE] Invio del valore: %d\n", val);

        /* TBD: Inviare il valore "val" tramite la coda di messaggi */
        
        //Creo il messaggio
        messaggio msg;
        msg.type= MSGTYPE;
        msg.valore = val;
        
        //Sendo il mess
        msgsnd(id_coda,&msg,sizeof(messaggio)-sizeof(long),0);

    }
}

void Consumatore(MonitorBufferSingolo* b, int id_coda) {

    /* TBD: Effettuare una consumazione su MonitorBufferSingolo con
            la funzione "consumazione_buffer_singolo", prelevando "numero_elementi"
     */

    int numero_elementi = consumazione_buffer_singolo(b);

    printf("[CONSUMATORE] Totale elementi da ricevere: %d\n", numero_elementi);

    for(int i=0; i<numero_elementi; i++) {

        /* TBD: Ricevere il valore "val" tramite la coda di messaggi */
        
        //Creo mess
        messaggio msg;
        
        //Ricevo mess
        msgrcv(id_coda,&msg,sizeof(messaggio)-sizeof(long),0,0);

        int val = msg.valore;

        printf("[CONSUMATORE] Ricezione del valore: %d\n", val);
    }
}
