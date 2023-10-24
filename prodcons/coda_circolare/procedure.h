#ifndef _PROCEDURE_H_
#define _PROCEDURE_H_

#define SPAZIO_DISPONIBILE 0
#define MESSAGGIO_DISPONIBILE 1
#define MUTEX_P 2
#define MUTEX_C 3

#define DIM_BUFFER 3

#define NUM_PRODUTTORI 5
#define NUM_CONSUMATORI 5

struct codaCircolare{

	int buffer[DIM_BUFFER];
	int testa;
	int coda;

};

void Produttore(struct codaCircolare*,int);

void Consumatore(struct codaCircolare*,int);


#endif
