#include "header.h"

void inizializza(ProdCons* pc){
	
	for (int i=0;i<DIM;i++){
		pc->stato[i] = LIBERO;
	}
	
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
	
		//Cerco la cella dove produrre
		int index = 0;
		while (index<DIM && pc->stato[index] != LIBERO){
			index++;
		}
		
		//Setto la cella a inuso
		pc->stato[index] = INUSO;
	
	//RILASCIO SEMAFORO DEI PRODUTTORI
	Signal_Sem(id_sem,MUTEX_P);
		
	//Produco
	int val = rand()%215 +1;
	pc->buffer[index] = val;
	pc->stato[index] = OCCUPATO;
	printf("[PRODUTTORE] Prodotto %d\n",val);
		
		
	//Segnalo che possono consumare
	Signal_Sem(id_sem,MESSAGGIO_DISPONIBILE);

}

void consuma(int id_sem,ProdCons* pc){

	//HO MESS?
	Wait_Sem(id_sem,MESSAGGIO_DISPONIBILE);
	
	//POSSO CONSUMARE O QUALCUNO GIA STA OCCUPANDO VETTORE?
	Wait_Sem(id_sem,MUTEX_C);
	
		//Cerco dove consumare
		int index = 0;
		while (index<DIM && pc->stato[index] != OCCUPATO){
			index++;
		}
		
		pc->stato[index] = INUSO;
		
	//Segnalo ad altri consumatori che possono consumare
	Signal_Sem(id_sem,MUTEX_C);
	int val = pc->buffer[index];
	pc->buffer[index] = LIBERO;
	
	printf("[CONSUMATORE] Consumato %d\n",val);
		
	
	//VAI PRODUCETE
	Signal_Sem(id_sem,SPAZIO_DISPONIBILE);

	
}

	
