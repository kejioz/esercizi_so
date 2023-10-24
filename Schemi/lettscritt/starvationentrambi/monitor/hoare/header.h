#ifndef _PROCEDURE_H_
#define _PROCEDURE_H_

#include "monitor_hoare.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

#define NUM_LETTORI 10
#define NUM_SCRITTORI 10

#define CV_LETTORI 0
#define CV_SCRITTORI 1


typedef struct{

	//Dato da leggere
	int valore;	

	//Variabili per la sincronizzazione
	int numero_lettori;
	int numero_scrittori;
	
	//Monitor
	Monitor m;
	
}LettScritt;

void inizializza(LettScritt* ls);
void pulisci(LettScritt* ls,int id_shm);
void IniziaLettura(LettScritt* ls);
void FineLettura(LettScritt* ls);
void IniziaScrittura(LettScritt* ls);
void FineScrittura(LettScritt* ls);
void lettore(LettScritt* ls);
void scrittore(LettScritt* ls);

#endif
