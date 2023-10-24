#include "header.h"
#include "monitor_signal_continue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>


void inizializza_prodcons(ProdCons* pc){

	//Mi occupo prima della coda
	pc->testa = 0;
	pc->coda = 0;
	pc->elems = 0;
	
	//Poi delle variabili di utility
	pc->numero_liberi = DIMENSIONE_CODA;
	pc->numero_occupati = 0;
	
	//Inizializzo il monitor con 2 variabili condition
	init_monitor (&(pc->m),2);
	printf(" -- Sistema -- Inizializzata la struttura prodcons. Testa:0 \t Coda:0 \t Elems:0 \t Liberi:%d\t Occupati:0\t Condvar:2\n",DIMENSIONE_CODA);
	
}

void rimuovi_prodcons(ProdCons* pc, int id_shm){

	//Per prima cosa rimuovo il monitor
	remove_monitor(&(pc->m));
	
	//Poi dealloco la shared memory
	shmctl(id_shm,IPC_RMID,0);
	
	printf(" -- Sistema -- Monitor e shm prodcons correttamente cleanati\n");

}

void produci (ProdCons* pc){
	//Per prima cosa devo entrare nel monitor
	enter_monitor(&(pc->m));
	
		//Sono dentro al monitor. Check sulla variabile condition CON IL WHILE!! SIG CONTINUE	
		while (pc->elems == DIMENSIONE_CODA){
			
			//Mi metto in wait
			printf(" [PRODUTTORE] Attendo. La coda è piena.\n");
			
			wait_condition(&(pc->m),CV_PRODUTTORI);
			
			printf(" [PRODUTTORE] Posso produrre finalmente.\n");
		
		}
	
		sleep(1);
		//Una volta che sono qui posso finalmente produrre il messaggio
		
		//Creazione messaggio
			messaggio msg;
			int i;
			
			//Tipo
			msg.type = MSG_TYPE;
			
			//Stringa casuale
			for (i=0;i<DIMENSIONE_STRINGA-1;i++){
			
				msg.stringa[i] = 'a' + (rand() % 26);
				
			}
			
			msg.stringa[i] = '\0';
			
			//Array interi casuali
			for (i=0;i<2;i++){
				msg.array[i] = rand() % 10;
			}
			
			//Variabile posta a 0
			msg.valore = 0;
		
		//Metto in coda il messaggio
		pc->messaggi[pc->coda] = msg;
		
		//Aumento elems e varie cose
		pc->elems = (pc->elems+1);
		pc->coda = (pc->coda+1) % DIMENSIONE_CODA;
		(pc->numero_liberi)--;
		(pc->numero_occupati)++;
		
		//Segnalo ai consumatori che è possibile consumare
		printf(" [PRODUTTORE] Ho prodotto il messaggio %s. Segnalo ai consumatori la disp.\n",msg.stringa);
		signal_condition(&(pc->m),CV_CONSUMATORI);
		
	//Posso uscire dal monitor
	leave_monitor(&(pc->m));
}


messaggio consuma (ProdCons* pc){

	//Per prima cosa devo entrare nel monitor
	enter_monitor(&(pc->m));
	
		//Sono dentro al monitor. Check sulla variabile condition CON IL WHILE!! SIG CONTINUE	
		while (pc->elems == 0){
			
			//Mi metto in wait
			printf(" [CONSUMATORE] Attendo. La coda è vuota.\n");
			
			wait_condition(&(pc->m),CV_CONSUMATORI);
			sleep(1);
			
			printf(" [CONSUMATORI] Posso consumare finalmente.\n");
		
		}
	
		sleep(1);
		//Una volta che sono qui posso finalmente consumare il messaggio
		
		//Consumazione messaggio
			messaggio msg = pc->messaggi[pc->testa];
		
		//Decremento le varie robe
		pc->elems = (pc->elems-1);
		pc->testa = (pc->testa+1) % DIMENSIONE_CODA;
		(pc->numero_liberi)++;
		(pc->numero_occupati)--;
		
		//Segnalo ai produttori che è possibile produrre
		printf(" [CONSUMATORE] Ho consumato il messaggio %s. Segnalo ai produttori la disp.\n",msg.stringa);
		signal_condition(&(pc->m),CV_PRODUTTORI);
		
	//Posso uscire dal monitor
	leave_monitor(&(pc->m));
	
	//Ritorno il messaggio
	return msg;

}

void Produttore(ProdCons* pc){
	
	//Ogni produttore deve produrre 4 messaggi
	int i;
	for (i=0;i<4;i++){
		produci(pc);
		printf(" --- Sistema --- Il produttore %d ha finito di produrre.\n",i+1);
	}
	

}

void Consumatore(ProdCons* pc,int id_queue){

	//Ogni consumatore deve consumare 4 messaggi
	int i;
	messaggio msg;
	for (i=0;i<4;i++){
	
		msg = consuma(pc);
		printf(" --- Sistema --- Il consumatore %d ha finito di consumare.\n",i+1);
		
		//Il messaggio lo mando sulla coda
		msgsnd(id_queue,(void*)&msg,sizeof(messaggio)-sizeof(long),0);
		printf("[CONSUMATORE] Inoltrato correttamente il messaggio alla coda dei filtri\n");
	}
	
}

void remove_msgqueue(int id_queue){

	msgctl(id_queue,IPC_RMID,0);

}

void Filtro(int id_queue_rec,int id_queue_send){

	messaggio ricevuto;
	
	for (int i=0;i<8;i++){
	
		//Ricezione del messaggio dalla coda gen_filtro
		printf("[FILTRO] Ricezione messaggio...\n");
		sleep(1);
		msgrcv(id_queue_rec, (void*)&ricevuto,sizeof(messaggio)-sizeof(long),MSG_TYPE,0);
		
		printf("[FILTRO] Ricevuto messaggio %s correttamente! \n",ricevuto.stringa);
		
		//Effettuo la ricerca del carattere x
		if (strchr(ricevuto.stringa,'x')!= (char*)NULL){
			
			//Se trova il carattere, inoltro al checksum
			printf("[FILTRO] Il messaggio %s contiene il carattere 'x'! Inoltro al checksum!\n",ricevuto.stringa);
			msgsnd(id_queue_send,(void*)&ricevuto,sizeof(messaggio)-sizeof(long),0);
		}
		else{
			
			//Altrimenti lo ignoro
			printf("[FILTRO] Il messaggio %s va gettato.\n",ricevuto.stringa); 
			
		}
		
	}
	
	printf(" -- Sistema -- Il filtro ha finito il suo lavoro.\n");
	exit(0);
	
}

void Checksum(int id_queue_rec,int id_queue_send){

	messaggio ricevuto;
	
	for (int j=0;j<8;j++){
	
		int somma_caratteri=0;
		int somma_interi=0;
		int somma_totale=0;
		int i=0;
		
		//Ricezione del messaggio dalla coda filtro_checksum
		printf("[CHECKSUM] Ricezione messaggio...\n");
		sleep(1);
		int ret = msgrcv(id_queue_rec, (void*) &ricevuto,sizeof(messaggio)-sizeof(long),MSG_TYPE,IPC_NOWAIT);
		
		if (ret<0){
		
			if (errno == ENOMSG){
			
				printf("[CHECKSUM] No message available. I quit.\n");
				continue;
			
			}
			else{
				printf("[CHECKSUM] ERROR\n");
				exit(-1);
			}
		}
			
		printf("[CHECKSUM] Ricevuto messaggio %s correttamente! \n",ricevuto.stringa);
		
		//Calcolo somma caratteri e somma vettore
		
			//Somma caratteri
			while (ricevuto.stringa[i]!='\0'){
			
				somma_caratteri += ricevuto.stringa[i];
				i++;
			
			}
	
			//Somma interi
			for (i=0;i<2;i++){
				
				somma_interi += ricevuto.array[i];
				
			}
			
		//Inserisco le due somme nel valore
		somma_totale = somma_interi + somma_caratteri;
		printf("[CHECKSUM] Il totale del messaggio %s è %d. In particolare, la somma dei caratteri è %d, mentre la somma degli interi è %d\n",ricevuto.stringa,somma_totale,somma_caratteri,somma_interi);
		ricevuto.valore = somma_totale;
		
		//Mando il messaggio al visualizzatore
		msgsnd(id_queue_send,(void*)&ricevuto, sizeof(messaggio)-sizeof(long),0);
	}
	
	printf(" --Sistema -- Il checksum ha finito il suo lavoro.\n");
	exit(0);
	
}

void Visualizzatore(int id_queue){

	messaggio ricevuto;
	int i;
	
	for (int j=0;j<8;j++){
		sleep(1);
		//Ricezione del messaggio
		int ret = msgrcv(id_queue,(void*)&ricevuto,sizeof(messaggio)-sizeof(long),MSG_TYPE,IPC_NOWAIT);
		
		if (ret<0){
		
			if (errno == ENOMSG){
			
				printf("[VISUALIZZATORE] No message available. I quit.\n");
				continue;
				
			}
			else{
				printf("[VISUALIZZATORE] ERRORE");
				exit(-1);
			}
		}
		
		printf("[VISUALIZZATORE] STRINGA: %s\n ---------------- VALORI: %d %d \n ---------------- SOMMA: %d\n",ricevuto.stringa,ricevuto.array[0],ricevuto.array[1],ricevuto.valore);
	
	
	}
	
	printf(" --Sistema -- Il visualizzatore ha finito il suo lavoro.\n");
	exit(0);

}
