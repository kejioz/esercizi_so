#include "header.h"

void inizializza(LettScritt* ls){
	ls->numero_lettori = 0;
	ls->valore = 0;
}

void IniziaLettura(int id_sem, LettScritt* ls){

	//Mutex sul semaforo dei lettori
	Wait_Sem(id_sem,MUTEXL);
	
		ls->numero_lettori++;
		
		if (ls->numero_lettori == 1){
			Wait_Sem(id_sem,SYNCH);
		}
		
	Signal_Sem(id_sem,MUTEXL);
	
}

void FineLettura(int id_sem, LettScritt* ls){
	
	//Mutex sul semaforo dei lettori
	Wait_Sem(id_sem,MUTEXL);
	
		ls->numero_lettori--;
		
		if (ls->numero_lettori == 0){
			Signal_Sem(id_sem,SYNCH);
		}
		
	Signal_Sem(id_sem,MUTEXL);
	
}

void IniziaScrittura(int id_sem){

	//Mutex synch
	Wait_Sem(id_sem,SYNCH);
	
}

void FineScrittura(int id_sem){
	
	//Mutex synch
	Signal_Sem(id_sem,SYNCH);
	
}

void lettore(int id_sem, LettScritt* ls){
	sleep(rand()%4);
	IniziaLettura(id_sem, ls);
	
		printf("[LETTORE] Ho letto %d\n",ls->valore);
	
	FineLettura( id_sem, ls);
	
}

void scrittore(int id_sem, LettScritt* ls){

	sleep(rand()%4);
	IniziaScrittura( id_sem  );
	
		ls->valore = rand() %125;
		printf("[SCRITTORE] Ho scritto %d\n",ls->valore);
	
	FineScrittura( id_sem);
	
}

void pulisci(int id_sem,int id_shm){

	shmctl(id_shm,IPC_RMID,0);
	semctl(id_sem,0,IPC_RMID);
	printf(" -- SIS -- Risorse IPC rimosse \n");

}
