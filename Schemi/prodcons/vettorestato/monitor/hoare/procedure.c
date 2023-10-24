#include "header.h"

void inizializza(ProdCons* pc){
	
	for (int i=0;i<DIM;i++){
		pc->stato[i] = LIBERO;
	}
	
	init_monitor(&pc->m,2);
	pc->numero_liberi = DIM;
	pc->numero_occupati =0;
	
}


void pulisci(ProdCons*pc ,int id_shm){

	remove_monitor(&pc->m);
	shmctl(id_shm,IPC_RMID,0);
	printf(" -- SIS -- Risorse IPC rimosse \n");

}

void produci(ProdCons* pc){

	//Entro nel monitor
	enter_monitor(&pc->m);
	
		//Check variabile condition
		if (pc->numero_liberi == 0){
			wait_condition(&pc->m,CV_PRODUTTORI);
		}
	
		//Cerco la cella dove produrre
		int index = 0;
		while (index<DIM && pc->stato[index] != LIBERO){
			index++;
		}
		
		//Setto la cella a inuso
		pc->stato[index] = INUSO;
		pc->numero_liberi--;
	
	//RILASCIO X PRODUZIONE
	leave_monitor(&pc->m);
		
	//Produco
	int val = rand()%215 +1;
	pc->buffer[index] = val;
	printf("[PRODUTTORE] Prodotto %d\n",val);
	
		
	//Rientro
	enter_monitor(&pc->m);
	
		pc->stato[index] = OCCUPATO;
		pc->numero_occupati++;
		signal_condition(&pc->m,CV_CONSUMATORI);
		
	leave_monitor(&pc->m);

}

void consuma(ProdCons* pc){

	//Entro nel monitor
	enter_monitor(&pc->m);
	
		//Check variabile condition
		if (pc->numero_occupati == 0){
			wait_condition(&pc->m,CV_CONSUMATORI);
		}
	
		//Cerco la cella dove consumare
		int index = 0;
		while (index<DIM && pc->stato[index] != OCCUPATO){
			index++;
		}
		
		//Setto la cella a inuso
		pc->stato[index] = INUSO;
		pc->numero_occupati--;
	
	//RILASCIO X CONSUMAZIONE
	leave_monitor(&pc->m);
		
	//Consumo
	int val = pc->buffer[index];
	
	printf("[CONSUMATORE] Consumato %d\n",val);
	
		
	//Rientro
	enter_monitor(&pc->m);
	
		pc->stato[index] = LIBERO;
		pc->numero_liberi++;
		signal_condition(&pc->m,CV_PRODUTTORI);
		
	leave_monitor(&pc->m);

	
}

	
