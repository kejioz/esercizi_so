#include "header.h"


void inizializzaPosti(structposti* p){

	int i;
	for (i=0;i<DIMENSIONE_VETTORE;i++){
		p->posti[i].stato = LIBERO;
		p->posti[i].id_cliente = -1;
	}
	
	printf(" /// SISTEMA /// Inizializzato vettore di posti a LIBERO\n");

}

void cliente(int id_sem,structposti* p,int* disponibilita,pid_t pid){

	int i,numero_prenotazioni;
	//Attendo da 0 a 5 secondi
	sleep(rand()%6);
	
	//Provo a prenotare posti
	numero_prenotazioni = (rand() % 4) +1;
	
	printf("[CLIENTE %d] Provo ad effettuare %d prenotazioni.\n",pid,numero_prenotazioni);
	
	for (i=0;i<numero_prenotazioni;i++){
	
		//Avviso il visualizzatore che sto entrando
		//Wait_Sem(id_sem,SYNCH);
		//Per accedere alla disponibilita devo prima acquisire il mutex
		Wait_Sem(id_sem,MUTEXDISPONIBILITA);
		
		//printf("[CLIENTE %d] Acquisito il mutex sulla disponibilita:%d \n",pid,*disponibilita);
	
			if (*disponibilita == 0){
				
				printf("[CLIENTE %d] ABORT - DISPONIBILITA ESAURITA\n",pid);
				Signal_Sem(id_sem,MUTEXDISPONIBILITA);
				exit(1);
				
			}
			
			//Se c'è disponibilità, allora posso effettuare la prenotazione
			//Devo trovare il primo posto libero. Acquisisco il mutex su tutto il vettore
			Wait_Sem(id_sem,MUTEXVETTORE);
				
				//printf("[CLIENTE %d] Acquisisco il mutex sul vettore\n",pid);
			
				//A questo punto posso scorrere il vettore e cercare il primo libero
				int index=0;
				
				while ((index < DIMENSIONE_VETTORE) && ((p->posti[index].stato)!=LIBERO)){
					
					index++;
					
				}
				
				//printf("[CLIENTE %d] Posto libero trovato!\n",pid);
				
				//A questo punto ho trovato dove posso produrre. Setto a inaggiorn
				p->posti[index].stato = IN_AGGIORNAMENTO;
				//Inoltre decremento il numero di posti disponibili
				(*disponibilita)--;
				Signal_Sem(id_sem,MUTEXDISPONIBILITA);
				
			//Posso rilasciare il semaforo del vettore per effettuare la produzione
			Signal_Sem(id_sem,MUTEXVETTORE);
			
		
		
		//A questo punto posso effettuare la produzione e settare stato a occupato
		sleep(2);
		p->posti[index].id_cliente = pid;
		p->posti[index].stato = OCCUPATO;
		
		//Avviso il visualizzatore che può visualizzare
		Signal_Sem(id_sem,SYNCH);
		
	}
	
}

void visualizzatore(int id_sem,structposti*p,int* disponibilita){

	int i;
	while (1){
		
		//Avviso i produttori che mi serve visualizzare
		Wait_Sem(id_sem,SYNCH);
		
		printf(" ---------------\n DISPONIBILITA: %d \n",*disponibilita);
		
		for (i=0;i<DIMENSIONE_VETTORE;i++){
		
			char stato[20];
			int id_cliente =0;
			if (p->posti[i].stato == IN_AGGIORNAMENTO){
				strcpy(stato,"IN AGGIORNAMENTO");
			}else if (p->posti[i].stato == OCCUPATO){
				strcpy(stato,"OCCUPATO");
			}else if (p->posti[i].stato == LIBERO){
				strcpy(stato,"LIBERO");
			}
			
			if (p->posti[i].id_cliente != -1){
				id_cliente = p->posti[i].id_cliente;
			}
			
			printf( "Posto : %d \t Stato : %s \t Cliente : %d \n",i,stato,id_cliente);		
		
		}
		
		sleep(2);
		//Avviso i produttori che possono fare quello che cazzo vogliono
		//Signal_Sem(id_sem,SYNCH);
		
	}

}
