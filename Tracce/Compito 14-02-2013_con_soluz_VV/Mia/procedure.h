#ifndef _PROCEDURE_H_
#define _PROCEDURE_H_

#define MUTEX 0	// MUTEX PER IL NUMERO DI LETTORI
#define APPELLO 1	//Per l'accesso della risorsa prossimo appello
#define PRENOTATI 2	//Per l'accesso della risorsa numero prenotati

//Per ftok
#define PATH "."
#define ID 'S'
#define ID_SEM 'P'

#define DATA_1 "14/02/2013"
#define DATA_2 "05/03/2013"
#define DATA_3 "02/04/2013"

typedef struct{

	char prossimo_appello[20];
	int numero_prenotati;
	
	//Variabile per la sincronizzaione
	int numero_lettori;

}esame_t;

void inizio_lettura(int sem, esame_t* esame);
void fine_lettura(int sem,esame_t* esame);
void inizio_scrittura(int sem);
void fine_scrittura(int sem);
void accedi_prenotati(int sem);
void lascia_prenotati(int sem);
void Docente(int sem,esame_t* esame);
void Studente(int sem,esame_t* esame);

#endif
