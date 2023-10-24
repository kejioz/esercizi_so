#ifndef _PROCEDURE_H_
#define _PROCEDURE_H_

#include "semafori.h"

#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define FTOK_PATH "."
#define FTOK_POSTI 'a'
#define FTOK_DISP 'b'
#define FTOK_SEM 'c'

#define NUMERO_PROCESSI 51
#define NUMERO_CLIENTI 50
#define NUMERO_VISUALIZZATORI 1

#define DIMENSIONE_VETTORE 80

#define LIBERO 0
#define OCCUPATO 1
#define IN_AGGIORNAMENTO 2

#define MUTEXVETTORE 0
#define MUTEXDISPONIBILITA 1
#define SYNCH 2

typedef struct{

	unsigned id_cliente;
	unsigned stato;

}posto;

typedef struct{

	//Vettore di posti
	posto posti[DIMENSIONE_VETTORE];
	
}structposti;

void inizializzaPosti(structposti* p);
void cliente(int id_sem,structposti* p,int* disponibilita,pid_t pid);
void visualizzatore(int id_sem,structposti*p,int* disponibilita);

#endif
