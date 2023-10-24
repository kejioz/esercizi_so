#include "header.h"

void inizializza(ProdCons* pc){
	
	pc->elems = 0;
	pc->testa = 0;
	pc->coda = 0;
	
}


void pulisci(int id_sem,int id_shm){

	shmctl(id_shm,IPC_RMID,0);
	semctl(id_sem,0,IPC_RMID);
	printf(" -- SIS -- Risorse IPC rimosse \n");

}

void produci(int id_sem,ProdCons* pc){

	//Per prima cosa devo vedere se ho spazio
	Wait_Sem(id_sem,SPAZIO_DISPONIBILE);
	
	//GESTISCO COMPETIZIONE CON ALTRI PRODUTTORI
	Wait_Sem(id_sem,MUTEX_P);
	
		//Produco
		int val = rand() %215;
		pc->buffer[pc->coda] = val;
		pc->elems++;
		pc->coda = (pc->coda +1) % DIM;
		printf("[PRODUTTORE] Prodotto %d\n",val);
		
	//Segnalo che altri possono produrre
	Signal_Sem(id_sem,MUTEX_P);
		
	//Segnalo che possono consumare
	Signal_Sem(id_sem,MESSAGGIO_DISPONIBILE);

}

void consuma(int id_sem,ProdCons* pc){

	//HO MESS?
	Wait_Sem(id_sem,MESSAGGIO_DISPONIBILE);
	
	//POSSO CONSUMARE O QUALCUNO GIA STA OCCUPANDO VETTORE?
	Wait_Sem(id_sem,MUTEX_C);
	
		//Consuma
		int val = pc->buffer[pc->testa];
		pc->elems--;
		pc->testa = (pc->testa +1) % DIM;
		printf("[CONSUMATORE] Consumato %d\n",val);
		
	//CONSUMATE VA
	Signal_Sem(id_sem,MUTEX_C);
	
	//VAI PRODUCETE
	Signal_Sem(id_sem,SPAZIO_DISPONIBILE);

	
}

	
