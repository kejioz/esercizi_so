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

#define CV_PRODUTTORI 0
#define CV_CONSUMATORI 1

#define LIBERO 0
#define INUSO 1
#define OCCUPATO 2

#define DIM 3

#define NUM_PRODUTTORI 10
#define NUM_CONSUMATORI 10

typedef struct{

	//Dato da leggere
	int buffer[DIM];
	int stato[DIM];
	
	//Variabili di supporto
	int numero_occupati;
	int numero_liberi;
	
	//Monitor 
	Monitor m;
	
}ProdCons;

void inizializza(ProdCons* pc);
void pulisci(ProdCons* pc,int id_shm);
void produci(ProdCons* pc);
void consuma(ProdCons* pc);

#endif
