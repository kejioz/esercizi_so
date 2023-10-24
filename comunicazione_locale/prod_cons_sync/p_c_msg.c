/*********PRODUTTORE-CONSUMATORE MEDIANTE SCAMBIO DI MESSAGGI******/
/*Il programma gestisce la comunicazione tra due processi, un processo
  produttore ed un processo consumatore,in un modello a memoria locale.
  Lo scambio avviene mediante protocollo sincrono realizzato a partire
  da primitive asincrone e con una comunicazione indiretta.Il program
  ma fa uso di tre mailbox:
  MB1-->SCAMBIO MSG DI SERVIZIO
  MB2-->SCAMBIO MSG DI SERVIZIO
  MB3-->SCAMBIO MSG DA TRASMETTERE

  Header file:header.h
  Programma chiamante:p_c_msg.c
  Modulo delle procedure:procedure.c
  Direttive per la compilazione dei moduli:Makefile
*/

#include <stdio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include "header.h"


int main(){

	int k,status,queue;
	pid_t pid;

	// PRENDO IL DESCRITTORE DELLA CODA SULLA QUALE VOGLIO COMUNICARE EFFETTIVAMENTE
	queue=msgget(IPC_PRIVATE,IPC_CREAT|0664);

	// INIZIALIZZO LE DUE CODE DI SERVIZIO PER RTS E OTS
	initServiceQueues();

	// GENERO PRODUTTORE E CONSUMATORE
	pid=fork();
	if (pid==0)  {			
		
		//PRODUTTORE
		printf("sono il produttore. Il mio pid %d \n",getpid());
		sleep(2);
		Produttore(queue,"www.unina.it");	//Mando il messaggio sulla coda queue
		_exit(0);
		
	} else {
		pid=fork();
				
	 	if (pid==0){
	 	
	 	//CONSUMATORE
			printf("sono il figlio consumatore. Il mio pid %d \n",getpid());
			sleep(1);
        		Consumatore(queue);
			_exit(0);
		}
	}

	// attesa di terminazione
	for (k=0; k<2;k++){
		pid=wait(&status);
		if (pid==-1)
			perror("errore");
		else
			printf ("Figlio n.ro %d e\' morto con status= %d\n",pid,status>>8);
	}

	// deallocazione code
	msgctl(queue,IPC_RMID,0);
	removeServiceQueues();
	
	return 1;
}
