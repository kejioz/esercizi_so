#include "semafori.h"
#include "procedure.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

void inizio_lettura(int sem, esame_t* esame){
	
	//Per prima cosa voglio modificare il numero di lettori
	Wait_Sem (sem,MUTEX);
	
		//Aumento il numero di lettori dell'appello
		esame->numero_lettori++;
		//Controllo se sono il primo lettore dell'appello. Se è così metto in wait gli scrittori
		if (esame->numero_lettori == 1){
			
			//Sono il primo lettore, dico agli scrittori ( professori ) che sono dentro e non devono rompere
			Wait_Sem(sem,APPELLO);
			printf("[STUDENTE] BLOCCO I LETTORI\n");
			
		}
		
	//Rilascio la risorsa numero_lettori
	Signal_Sem (sem,MUTEX);
		
}

void fine_lettura(int sem, esame_t* esame){

	//Per prima cosa voglio decrementare il numero di lettori
	Wait_Sem(sem,MUTEX);
	
		esame->numero_lettori--;
	
		//Poi se sono l'ultimo lettore voglio dire agli scrittori che ho finito di usare la risorsa e possono scrivere
		if (esame->numero_lettori==0){
		
			Signal_Sem(sem,APPELLO);
			printf("[STUDENTE] SBLOCCO I LETTORI\n");
		}
	
	//A questo punto posso lasciare la risorsa numero lettori
	Signal_Sem(sem,MUTEX);		

}

void accedi_prenotati(int sem){

	Wait_Sem(sem,PRENOTATI);

}

void lascia_prenotati(int sem){

	Signal_Sem(sem,PRENOTATI);

}

void Studente(int sem,esame_t* esame){

	printf("[STUDENTE] Inizio la lettura...\n");
	
	sleep(rand()%9);
	
	inizio_lettura(sem,esame);
	
	char data_esame[20];
	
	strcpy(data_esame,esame->prossimo_appello);
	
	printf("[STUDENTE] Permesso acquisito, il prossimo appello è il %s.\n",data_esame);
	
	fine_lettura(sem,esame);
	
	printf("[STUDENTE] Provo a prenotarmi...\n");
	
	accedi_prenotati(sem);
	
	esame->numero_prenotati++;
	
	printf("[STUDENTE] Prenotazione effettuata all'esame %s\n",data_esame);
	
	lascia_prenotati(sem);
		

}

void inizia_scrittura(int sem){

	//Solo wait e signal sul semaforo per la scrittura
	Wait_Sem(sem,APPELLO);

}

void fine_scrittura(int sem){

	Signal_Sem(sem,APPELLO);
	
}

void Docente(int sem,esame_t* esame){

	int i;
	char nuovadata[20];
	for (i=0;i<3;i++){
	
		if (i==0)
			strcpy(nuovadata,DATA_1);
		else if (i==1)
			strcpy(nuovadata,DATA_2);
		else if (i==2)
			strcpy(nuovadata,DATA_1);
			
		inizia_scrittura(sem);
		
		strcpy(esame->prossimo_appello,nuovadata);
		
		printf("[DOCENTE] Aggiornata data di appello a %s\n",nuovadata);
		
		fine_scrittura(sem);
		
		sleep(3);
		
		accedi_prenotati(sem);
		
		printf("[DOCENTE] Il numero di prenotati all'appello %s è di %d.\n",nuovadata,esame->numero_prenotati);
		
		esame->numero_prenotati = 0;
		
		lascia_prenotati(sem);
		
	
	}

}
