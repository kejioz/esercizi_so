#ifndef _HEADER_
#define _HEADER_

#include <pthread.h>


/* TBD: Definire qui una struttura dati per i messaggi */

typedef struct {

    int valore;
    
    /* TBD: Estendere questa struttura, in modo da realizzare la 
     *      mutua esclusione per l'accesso al <valore
     */
     
     //Mutex per la mutua esclusione; Non mi servono le condition variables
     pthread_mutex_t mutex;
     

} BufferMutuaEsclusione;

typedef struct {

	long type;
	int msg;

} messaggio;

#define NUM_THREADS 10
#define MSG_CLIENTS 5
#define NUM_CLIENT 2
#define NUM_SERVER 1

#define FTOK_PATH_CODA "."
#define FTOK_CHAR_CODA 'a'

#endif
