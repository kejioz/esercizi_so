#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/types.h>

#include "pubsub.h"

void subscriber(int id_coda_registrazioni, long topic);

int main() {

    srand(getpid());

	pid_t pid;
    
    key_t chiave_coda_registrazioni = ftok(FTOK_PATH,FTOK_CHAR_CODA_REGISTRAZIONI);

    int id_coda_registrazioni; /* TBD: Creare una coda di messaggi di registrazione */
    
    id_coda_registrazioni = msgget(chiave_coda_registrazioni,IPC_CREAT|0664);


    for(int i=0; i<NUM_SUBSCRIBERS_PER_TOPIC; i++) {

        printf("[SUBSCRIBER] Avvio processo subscriber su topic 1\n");

        /* TBD: 
            Creare un processo figlio, che esegua la funzione "subscriber".
            Passare come parametri lo ID della coda per le registrazioni, e il valore TOPIC1.
        */
        
        pid = fork();
        
        if (pid == 0){
        
        	subscriber(id_coda_registrazioni,TOPIC1);
        	exit(0);
        
        }
        
    }


    for(int i=0; i<NUM_SUBSCRIBERS_PER_TOPIC; i++) {

        printf("[SUBSCRIBER] Avvio processo subscriber su topic 2\n");

        /* TBD: 
            Creare un processo figlio, che esegua la funzione "subscriber".
            Passare come parametri lo ID della coda per le registrazioni, e il valore TOPIC2.
        */
        
        pid = fork();
        
        if (pid == 0){
        
        	subscriber(id_coda_registrazioni,TOPIC2);
        	exit(0);
        
        }
    }


    printf("[SUBSCRIBER] Attesa terminazione processi figli\n");

    /* TBD: Attendere la terminazione dei processi figli */
    for (int i=0;i<NUM_SUBSCRIBERS_PER_TOPIC*2;i++){
    	wait(NULL);
    	printf("[ SUBSCRIBER ] Figlio %d terminato \n",i+1);
    }

}

void subscriber(int id_coda_registrazioni, long topic) {


    key_t chiave_coda_messaggi_subscriber = IPC_PRIVATE;

    int id_coda_messaggi_subscriber; /* TBD: Creare una coda di messaggi privata del subscriber */
    
    id_coda_messaggi_subscriber = msgget(chiave_coda_messaggi_subscriber,IPC_CREAT|0664);

    messaggio_registrazione registrazione;

    /* TBD: 
        Inviare un messaggio di registrazione al broker.
        Includere nel messaggio lo ID della coda di messaggi privata.
    */
    
    registrazione.type = topic;
    registrazione.id_coda = id_coda_messaggi_subscriber;
    
    msgsnd(id_coda_registrazioni,&registrazione,sizeof(messaggio_registrazione)-sizeof(long),0);

    for(int i=0; i<NUM_MESSAGGI*NUM_PUBLISHERS_PER_TOPIC; i++) {

        messaggio_valore messaggio;

        /* TBD: Ricevere un messaggio dal broker */
        
        msgrcv(id_coda_messaggi_subscriber,&messaggio,sizeof(messaggio_valore)-sizeof(long),topic,0);

        int valore = messaggio.valore;

        printf("[SUBSCRIBER] Ricevuto messaggio: topic=%ld, valore=%d\n", topic, valore);

    }

    /* TBD: Deallocare la coda di messaggi privata del subscriber */
    msgctl(id_coda_messaggi_subscriber,IPC_RMID,0);
    printf("[SUBSCRIBER] Coda privata deallocata con successo.\n");

}

