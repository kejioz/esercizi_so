#include "header.h"

void inizializza(LettScritt* ls){

	ls->numero_lettori = 0;
	ls->numero_scrittori = 0;
	
	init_monitor(&ls->m,2);
}

void IniziaLettura(LettScritt* ls){

	//Entro nel monitor
	enter_monitor(&ls->m);
	
		//Sono dentro. Check sulla condition variable
		while (ls->numero_scrittori > 0){
			wait_condition(&ls->m,CV_LETTORI);
		}
		
		//Quando esco posso aumentare il numero di lettori
		ls->numero_lettori++;
	
	//Lascio il monitor
	leave_monitor(&ls->m);		
	
}

void FineLettura(LettScritt* ls){
	
	
	//Ho letto. Entro nel monitor
	enter_monitor(&ls->m);
	
		ls->numero_lettori--;
		
		if (ls->numero_lettori == 0){
			
			//Se sono l'ultimo lettore segnalo gli scrittori. E' SIG CONTINUE, SO VER O SCRITT
			signal_condition(&ls->m,CV_SCRITTORI);
			
		}
		
	leave_monitor(&ls->m);

	
}

void IniziaScrittura(LettScritt* ls){

	//ENtro nel onitor
	enter_monitor(&ls->m);
	
		//Check sulla cv
		while (ls->numero_lettori > 0 || ls->numero_scrittori > 0){
		
			wait_condition(&ls->m,CV_SCRITTORI);
			
		}
	
	ls->numero_scrittori++;
		
	//Lascio il monitor
	leave_monitor(&ls->m);
	
}

void FineScrittura(LettScritt* ls){
	
	//Entro nel monitor
	enter_monitor(&ls->m);
	
		ls->numero_scrittori--;
		
		//Se ci sono scrittori sospesi sulla cv scrittori, segnalo loro, altrimenti lettori
		if (queue_condition(&ls->m,CV_SCRITTORI)>0){
		
			signal_condition(&ls->m,CV_SCRITTORI);
			
		}else{
		
			signal_all(&ls->m,CV_LETTORI);
			
		}
	
		
	leave_monitor(&ls->m);
	
}

void lettore(LettScritt* ls){

	sleep(rand()%4);
	IniziaLettura(ls);
	
		printf("[LETTORE] Ho letto %d\n",ls->valore);
	
	FineLettura(ls);
	
}

void scrittore(LettScritt* ls){

	sleep(rand()%4);
	IniziaScrittura(ls);
	
		ls->valore = rand() %125;
		printf("[SCRITTORE] Ho scritto %d\n",ls->valore);
	
	FineScrittura(ls);
	
}

void pulisci(LettScritt* ls,int id_shm){

	shmctl(id_shm,IPC_RMID,0);
	remove_monitor(&ls->m);
	printf(" -- SIS -- Risorse IPC rimosse \n");

}
