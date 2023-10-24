#include "header.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(){

	//Variabili utility
	int i;
	key_t key_shmem;
	key_t key_sem;
	key_t key_queue;
	LettScritt* ls;
	int id_shm,id_sem,id_queue;
	
	//Keys
	key_shmem = ftok(FTOK_PATH,FTOK_CHAR_SHM);
	key_sem = ftok(FTOK_PATH,FTOK_CHAR_SEM);
	key_queue = ftok(FTOK_PATH,FTOK_CHAR_QUE);
	
	
	//Descrittori 
	
		//Shmem
		id_shm = shmget(key_shmem,sizeof(LettScritt),IPC_CREAT|0664);
		
			//Check id shmem
			if (id_shm < 0 ){
				
				perror("Errore shmget\n");
				exit(1);
			
			}
			
			//Attach alla shmem
			ls = (LettScritt*) shmat(id_shm,0,0);
			
			//Check attach alla shmem
			if (ls == (void*) -1){
			
				perror("Errore shmat\n");
				exit(1);
			
			}
						
			printf(" --- LETTORE --- Attach alla shmem avvenuto con successo \n");
			
		//Sem
		id_sem = semget(key_sem,4,IPC_CREAT|0664);
		
			//Check id sem
			if (id_sem < 0 ){
				
				perror("Errore semget\n");
				exit(1);
			
			}
			
			printf(" --- LETTORE --- Inizializzazione semafori avvenuta con successo \n");
			
		//Queue
		id_queue = msgget(key_queue,IPC_CREAT|0664);
	
			//Check id queue
			if (id_queue < 0 ){
				
				perror("Errore msgget\n");
				exit(1);
			
			}
			
			printf(" -- LETTORE-- Inizializzazione coda avvenuta con successo \n");
			
	Lettore(id_sem,ls,id_queue);


	exit(0);

}
