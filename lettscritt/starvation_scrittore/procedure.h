#ifndef _PROCEDURE_H_
#define _PROCEDURE_H_

#define MUTEXL 0
#define SYNCH 1
#define NUM_VOLTE 6

typedef long msg;
typedef struct {

	msg messaggio;
	int numlettori;
	
}Buffer;

void Lettore(int,Buffer*);

void Scrittore(int,Buffer*);


#endif
