#ifndef _PROCEDURE_H_
#define _PROCEDURE_H_

#include "semafori.h"

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

#define MUTEXL 0
#define SYNCH 1

#define NUM_LETTORI 10
#define NUM_SCRITTORI 10

typedef struct{

	//Dato da leggere
	int valore;	

	//Variabili per la sincronizzazione
	int numero_lettori;
	
}LettScritt;

void inizializza(LettScritt* ls);
void pulisci(int id_sem, int id_shm);
void IniziaLettura(int id_sem, LettScritt* ls);
void FineLettura(int id_sem, LettScritt* ls);
void IniziaScrittura(int id_sem);
void FineScrittura(int id_sem);
void lettore(int id_sem, LettScritt* ls);
void scrittore(int id_sem, LettScritt* ls);

#endif
