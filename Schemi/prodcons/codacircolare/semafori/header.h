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

#define MESSAGGIO_DISPONIBILE 0
#define SPAZIO_DISPONIBILE 1
#define MUTEX_P 2
#define MUTEX_C 3

#define DIM 3

#define NUM_PRODUTTORI 10
#define NUM_CONSUMATORI 10

typedef struct{

	//Dato da leggere
	int buffer[3];
	int testa;
	int coda;
	int elems;
	
}ProdCons;

void inizializza(ProdCons* pc);
void pulisci(int id_sem, int id_shm);
void produci(int id_sem,ProdCons* pc);
void consuma(int id_sem,ProdCons* pc);

#endif
