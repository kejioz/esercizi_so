#ifndef _PROCEDURE_H_
#define _PROCEDURE_H_

#include "semafori.h"

#define NUMERO_ELEMENTI_BUFFER 2

#define MUTEXL 0
#define MUTEXS 1
#define SYNCH 2	//Per mutua esclusione sia scrittori che lettori
#define MUTEX 3 //Per gli scrittori in mutua esclusione

#define NUMERO_SCRITTURE 5
#define NUMERO_LETTURE 5

#define NUMERO_LETTORI 5
#define NUMERO_SCRITTORI 3

#define MSG_TYPE 1

#define FTOK_PATH "."
#define FTOK_CHAR_SEM 'a'
#define FTOK_CHAR_SHM 'b'
#define FTOK_CHAR_QUE 'c'


typedef struct{
	
	char buffer[NUMERO_ELEMENTI_BUFFER];
	
	int numero_lettori;
	int numero_scrittori;

}LettScritt;

typedef struct{

	long type;
	char char1;
	char char2;
	
}message;

void IniziaLettura(int id_sem,LettScritt* ls);
void IniziaScrittura(int id_sem,LettScritt* ls);
void FineLettura(int id_sem,LettScritt* ls);
void FineScrittura(int id_sem,LettScritt* ls);
void Lettore(int id_sem,LettScritt* ls,int id_queue);
void Scrittore(int id_sem,LettScritt* ls);

#endif
