#include "header.h"

void inizializza(LettScritt* ls){
	ls->numero_lettori = 0;
	ls->numero_scrittori = 0;
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

void IniziaScrittura(int id_sem, LettScritt* ls){

	//Mi metto in coda per gli scrittori
	Wait_Sem(id_sem,MUTEXS);
		
		ls->numero_scrittori++;
		
		if (ls->numero_scrittori == 1){
		
			//Se sono il primo scrittore, devo mettere in wait tutti quanti i lettori
			Wait_Sem(id_sem,SYNCH);
			
		}
		
	Signal_Sem(id_sem,MUTEXS);
	
	//Mi metto in coda CON gli altri scrittori
	Wait_Sem(id_sem,MUTEX);
	
}

void FineScrittura(int id_sem, LettScritt* ls){
	
	//Dico agli altri scrittori che ho finito di scrivere e possono farlo loro
	Signal_Sem(id_sem,MUTEX);
	
	//Voglio cambiare il nm scritt
	Wait_Sem(id_sem,MUTEXS);
		
		ls->numero_scrittori--;
		
		if (ls->numero_scrittori == 0){
		
			//Se sono l'ultimo scrittore, devo avvisare i lettori
			Signal_Sem(id_sem,SYNCH);
			
		}
		
	Signal_Sem(id_sem,MUTEXS);
	
}

void lettore(int id_sem, LettScritt* ls){
	sleep(rand()%4);
	IniziaLettura(id_sem, ls);
	
		printf("[LETTORE] Ho letto %d\n",ls->valore);
	
	FineLettura( id_sem, ls);
	
}

void scrittore(int id_sem, LettScritt* ls){

	sleep(rand()%4);
	IniziaScrittura( id_sem,ls);
	
		ls->valore = rand() %125;
		printf("[SCRITTORE] Ho scritto %d\n",ls->valore);
	
	FineScrittura( id_sem,ls);
	
}

void pulisci(int id_sem,int id_shm){

	shmctl(id_shm,IPC_RMID,0);
	semctl(id_sem,0,IPC_RMID);
	printf(" -- SIS -- Risorse IPC rimosse \n");

}
