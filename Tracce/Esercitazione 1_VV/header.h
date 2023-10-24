#ifndef _PROCEDURE_H_
#define _PROCEDURE_H_

#define MUTEXL 0
#define SYNCH 1

#define MAX_STR_DIM 15

#define FTOK_PATH "."
#define FTOK_CHAR_SHM 'a'
#define FTOK_CHAR_SEM 'b'

typedef struct{
	
	//Variaibli per la gestione della starvation
	int numlettori;
	
	//Stringa
	char stringa [MAX_STR_DIM];

}LettScritt;

void IniziaLettura(int,LettScritt*);
void FineLettura(int,LettScritt*);
void IniziaScrittura(int;
void FineScrittura(int);

#endif
