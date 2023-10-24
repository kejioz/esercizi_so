#ifndef _PROCEDURE_H_
#define _PROCEDURE_H_

#define SPAZIO_DISPONIBILE 0
#define MESSAGGIO_DISPONIBILE 1
#define MUTEX_P 2
#define MUTEX_C 3

#define DIM_BUFFER 3

#define NUM_PRODUTTORI 5
#define NUM_CONSUMATORI 5

#define BUFFER_VUOTO 0
#define BUFFER_INUSO 1
#define BUFFER_PIENO 2

struct vettoreStato{

	int buffer[DIM_BUFFER];
	int stato[DIM_BUFFER];

};

void Produttore(struct vettoreStato*,int);

void Consumatore(struct vettoreStato*,int);


#endif
