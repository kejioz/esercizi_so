#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>


#include "header.h"

#define NUM_CLIENT 2

int main() {

	/* TBD: Allocare una coda di messaggi */
	key_t key_coda = ftok(".",'a');

    int id_coda = msgget(key_coda,IPC_CREAT|0664);
    
    if (id_coda<0){
    	perror("Errore msgget");
    	exit(1);
    }

	pid_t pid;
	
    for(int i=0; i<NUM_CLIENT; i++) {

        /* TBD: Creare i processi figli "client", e
         *      fargli eseguire il programma "main_client"
         */
         pid = fork();
         
         if (pid == 0){
         
         	execv("./main_client",(char* const)NULL);
         	
         	printf("Non dovrei essere qui\n");
         	exit(1);
        
         }
    }

	
    /* TBD: Creare i processi figli "server", e
     *      fargli eseguire il programma "main_server"
     */
	for (int i=0;i<NUM_SERVER;i++){
	
		pid = fork();
		
		if (pid == 0){
			
			execv("./main_server",(char* const)NULL);
		    printf("Non dovrei essere qui\n");
         	exit(1);
		}
	
	}

    /* TBD: Attendere la terminazione dei processi figli */
    for (int i=0;i<NUM_CLIENT+NUM_SERVER;i++){
    	wait(NULL);
    	printf("Processo %d terminato\n",i);
    }

    /* TBD: De-allocare la coda di messaggi */
    msgctl(id_coda,IPC_RMID,0);
    printf("Coda deallocata\n");


    return 0;

}
