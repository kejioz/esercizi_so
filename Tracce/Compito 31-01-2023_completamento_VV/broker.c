#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <pthread.h>

#include "pubsub.h"

#define MAX_TOPICS 2
#define MAX_SUBSCRIBERS 2

int code[MAX_TOPICS][MAX_SUBSCRIBERS];

void * gestisci_registrazioni(void * arg);
void * gestisci_messaggi(void * arg);

typedef struct{

	int id_coda_registrazioni;
	int id_coda_messaggi;

}idcode;

int main() {

    // inizializzazione lista di code
    for(int i=0; i<MAX_TOPICS; i++) {
        for(int j=0; j<MAX_SUBSCRIBERS; j++) {
            code[i][j] = 0;
        }
    }
    
    //Registrazioni

 	key_t chiave_coda_registrazioni = ftok(FTOK_PATH,FTOK_CHAR_CODA_REGISTRAZIONI);

    int id_coda_registrazioni; /* TBD: Creare una coda di messaggi di registrazione */
    
    id_coda_registrazioni = msgget(chiave_coda_registrazioni,IPC_CREAT|0664);

	//Messaggi	

    key_t chiave_coda_messaggi = ftok(FTOK_PATH,FTOK_CHAR_CODA_MESSAGGI);

    int id_coda_messaggi; /* TBD: Creare una coda di messaggi dei publisher */

	id_coda_messaggi = msgget(chiave_coda_messaggi,IPC_CREAT|0664);
	
	idcode* code = (idcode*) malloc (sizeof(idcode));
	code->id_coda_registrazioni=id_coda_registrazioni;
	code->id_coda_messaggi=id_coda_messaggi;


    /* TBD:
        Creare un thread che esegua la funzione "gestisci_registrazioni".
        Passare come parametro alla funzione gli id delle code.
     */
     
     pthread_t thread_registrazioni;
     
     pthread_create (&thread_registrazioni,NULL,gestisci_registrazioni,(void*) code);

    /* TBD:
        Creare un thread che esegua la funzione "gestisci_messaggi".
        Passare come parametro alla funzione gli id delle code.
     */
     
	pthread_t thread_messaggi;
     
    pthread_create (&thread_messaggi,NULL,gestisci_messaggi,(void*) code);

    /* TBD: Attendere la terminazione dei 2 thread */
    
    pthread_join(thread_registrazioni,NULL);
    printf("[BROKER] Thread registrazioni terminato\n");
    
    pthread_join(thread_messaggi,NULL);
    printf("[BROKER] Thread messaggi terminato\n");

    /* TBD: Deallocare le 2 code di messaggi */
    
    msgctl(id_coda_registrazioni,IPC_RMID,0);
    msgctl(id_coda_messaggi,IPC_RMID,0);
    free(code);
    printf("[BROKER] Risorse deallocate\n");
    
    pthread_exit(NULL);
}


void * gestisci_registrazioni(void * arg) {


    /* TBD: Completare il passaggio di parametri */
	idcode* codes = (idcode*) arg;
	int id_coda_registrazioni = codes->id_coda_registrazioni;

    for(int i=0; i<TOTALE_SUBSCRIBER; i++) {

        messaggio_registrazione messaggio;

        /* TBD: Ricevere un messaggio di registrazione */
		msgrcv(id_coda_registrazioni,&messaggio,sizeof(messaggio_registrazione)-sizeof(long),0,0);


        long topic = messaggio.type;
        int id_coda = messaggio.id_coda;

        printf("[BROKER] Ricevuto messaggio di registrazione: topic=%ld, id_coda=%d\n", topic, id_coda);


        // aggiungo il subscriber alla lista di code

        if(topic <= 0 || topic > MAX_TOPICS) {
            printf("[BROKER] Topic non valido\n");
            exit(1);
        }

        if(id_coda <= 0) {
            printf("[BROKER] Id coda non valido\n");
            exit(1);
        }

        int trovato = 0;
        for(int i=0; i<MAX_SUBSCRIBERS; i++) {
            if(code[topic-1][i] == 0) {
                code[topic-1][i] = id_coda;
                trovato = 1;
                break;
            }
        }

        if(trovato == 0) {
            printf("[BROKER] Numero massimo di subscriber raggiunto\n");
            exit(1);
        }

    }
    

    return NULL;
}


void * gestisci_messaggi(void * arg) {


    /* TBD: Completare il passaggio di parametri */
    idcode* codes = (idcode*) arg;
	int id_coda_messaggi = codes->id_coda_messaggi;


    for(int i=0; i<TOTALE_MESSAGGI; i++) {

        messaggio_valore messaggio;

        /* TBD: Ricevere un messaggio dai publisher  */
		msgrcv(id_coda_messaggi,&messaggio,sizeof(messaggio_valore)-sizeof(long),0,0);


        long topic = messaggio.type;
        int valore = messaggio.valore;

        printf("[BROKER] Ricevuto messaggio: topic=%ld, valore=%d\n", messaggio.type, messaggio.valore);



        // invio messaggio a tutti i subscriber registrati al topic

        if(topic <= 0 || topic > MAX_TOPICS) {
            printf("[BROKER] Topic non valido\n");
            exit(1);
        }

        for(int j=0; j<MAX_SUBSCRIBERS; j++) {

            int id_coda = code[topic-1][j];

            if(id_coda != 0) {

                /* TBD: Inviare il messaggio al subscriber in ascolto sulla coda "id_coda" */
				msgsnd(id_coda,&messaggio,sizeof(messaggio_valore)-sizeof(long),0);
				printf("[BROKER] Messaggio inoltrato: topic=%ld, valore=%d\n", messaggio.type, messaggio.valore);
            }
        }

    }

    return NULL;
}
