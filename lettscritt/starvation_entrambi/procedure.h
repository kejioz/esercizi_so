#ifndef _PROCEDURE_H_
#define _PROCEDURE_H_

typedef long msg;

typedef struct{

    msg messaggio;
    int numlettori;
    int numscrittori;

}Buffer;

#define SYNCH 0
#define MUTEXS 1
#define MUTEXL 2
#define MUTEX 3
#define NUM_VOLTE 3

void Lettore(int, Buffer*);
void Scrittore(int, Buffer*);
void IniziaLettura(int, Buffer*);
void FineLettura(int, Buffer*);
void IniziaScrittura(int, Buffer*);
void FineScrittura(int, Buffer*);

#endif