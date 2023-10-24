#include "header.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <time.h>


void IniziaLettura(int id_sem,LettScritt* ls){

	//Per prima cosa devo poter modificare il numero di lettori
	Wait_Sem(id_sem,MUTEXL);
	
	printf("[LETTORE] Inizio lettura...\n");
	
		//Acquisito il mutex della variabile numero_lettori, la incremento
		ls->numero_lettori++;
		
		if (ls->numero_lettori == 1){
			
			//Se sono il primo lettore vuol dire che devo far waitare gli scrittori
			Wait_Sem(id_sem,SYNCH);
			printf("[LETTORE] Sono il primo lettore, faccio attendere gli scrittori\n");
		
		}
		
	//La rilascio
	Signal_Sem(id_sem,MUTEXL);

}


void IniziaScrittura(int id_sem,LettScritt* ls){

	//Per prima cosa voglio modificare il numero di scrittori
	Wait_Sem(id_sem,MUTEXS);
	
	printf("[SCRITTORE] Inizio scrittura...\n");
	
		ls->numero_scrittori++;
		
		if (ls->numero_scrittori == 1){
		
			//Se sono il primo scrittore vuol dire che devo far waitare i lettori
			Wait_Sem(id_sem,SYNCH);
			printf("[SCRITTORE] Sono il primo scrittore, faccio attendere i lettori\n");
		
		}
		
	//Posso rilasciare il mutex per mettersi in coda per la scrittura
	Signal_Sem(id_sem,MUTEXS);
	
	//A questo punto faccio una wait per la mutua esclusione tra scrittori. Devo vedere se posso
	Wait_Sem(id_sem,MUTEX);

}



void FineLettura(int id_sem,LettScritt* ls){

	//Devo accedere alla variabile numero lettori
	Wait_Sem(id_sem,MUTEXL);
	
		ls->numero_lettori--;
		
		if (ls->numero_lettori == 0){
			
			//Se sono l'ultimo lettore, posso segnalare gli scrittori
			Signal_Sem(id_sem,SYNCH);
			printf("[LETTORE] Sono l'ultimo lettore, sveglio gli scrittori\n");
		
		}
		
	//Lascio la variabile numero lettori
	Signal_Sem(id_sem,MUTEXL);

	printf("[LETTORE] Fine lettura...\n");

}




void FineScrittura(int id_sem,LettScritt* ls){

	//Una volta scritto, posso permettere ad altri scrittori di scrivere
	Signal_Sem(id_sem,MUTEX);
	
	//A questo punto voglio decrementare il numero di scrittori quindi riacquisisco sem
	Wait_Sem(id_sem,MUTEXS);
	
		ls->numero_scrittori--;
		
		if (ls->numero_scrittori == 0 ){
		
			//Se sono l'ultimo scrittore, posso segnalare i lettori
			Signal_Sem(id_sem,SYNCH);
			printf("[SCRITTORE] Sono l'ultimo scrittore, sveglio i lettori\n");
			
		}

	//Lascio la variabile numero scrittori
	Signal_Sem(id_sem,MUTEXS);
	
	printf("[SCRITTORE] Fine scrittura...\n");

}



void Lettore(int id_sem,LettScritt* ls,int id_queue){

	int i;
	
	
	for (i=0;i<NUMERO_LETTURE;i++){
	
		srand(time(NULL)*i);
		IniziaLettura(id_sem,ls);
		
		
			sleep(1);
			//Posso leggere tranquillamente
			char char1 = ls->buffer[0];
			char char2 = ls->buffer[1];
			ls->buffer[0] = (ls->buffer[0]) + 32;
			ls->buffer[1] = (ls->buffer[1]) -32;
			char newchar1 = ls->buffer[0];
			char newchar2 = ls->buffer[1];
			printf("[LETTORE] Char1: %c \t Char2: %c \t NewChar1: %c \t NewChar2: %c \n",char1,char2,newchar1,newchar2);
		
		FineLettura(id_sem,ls);
		
		//Invio del messaggio sulla coda
		message msg;
		msg.type = MSG_TYPE;
		msg.char1 = newchar1;
		msg.char2 = newchar2;
		
		msgsnd(id_queue,&msg,sizeof(message)-sizeof(long),0);
		printf("[LETTORE] Spedito correttamente messaggio // %c %c // \n",newchar1,newchar2);
			
	}
	
	printf("[LETTORE] Tutte le letture sono terminate.\n");

}



void Scrittore(int id_sem,LettScritt* ls){
	
	int i;
	
	for (i=0;i<NUMERO_SCRITTURE;i++){
		srand(time(NULL)*i);
		IniziaScrittura(id_sem,ls);
		
			sleep(1);
			//Posso scrivere tranquillamente
			char char1 = 65 + (rand() % 26);
			char char2 = 97 + (rand() % 26);
			
			ls->buffer[0] = char1;
			ls->buffer[1] = char2;
			printf("[SCRITTORE] Scritto Char1: %c \t Char2: %c \n",char1,char2);
			
			
		FineScrittura(id_sem,ls);
	
	}
	
	printf("[SCRITTORE] Tutte le scritture sono terminate.\n");

}

