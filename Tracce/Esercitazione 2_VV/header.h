#ifndef _PROCEDURE_H_
#define _PROCEDURE_H_

#include "monitor_signal_continue.h"

#define DIMENSIONE_CODA 4
#define DIMENSIONE_STRINGA 10
#define NUMERO_PRODUTTORI 2
#define NUMERO_CONSUMATORI 2
#define NUMERO_FILTRI 1
#define NUMERO_CHECKSUM 1
#define NUMERO_VISUALIZZATORI 1

#define MSG_TYPE 1

#define CV_PRODUTTORI 0
#define CV_CONSUMATORI 1

typedef struct{

	long type;
	char stringa[DIMENSIONE_STRINGA];
	int array[2];
	int valore;

}messaggio;

typedef struct{
	
	//Gestione coda circolare
	messaggio messaggi[DIMENSIONE_CODA];
	int testa;
	int coda;
	int elems;
	
	//Monitor , variabili utility per le condizioni
	int numero_liberi;
	int numero_occupati;
	
	Monitor m;

}ProdCons;


//Generatore
void produci (ProdCons*);
messaggio consuma (ProdCons*);
void inizializza_prodcons(ProdCons*);
void rimuovi_prodcons(ProdCons*,int);
void Produttore (ProdCons*);
void Consumatore (ProdCons*,int);

//Deallocazione code messaggi
void remove_msgqueue(int);

//Filtro
void Filtro(int,int);
void Checksum(int,int);
void Visualizzatore(int);

#endif
