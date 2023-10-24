#include "header.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

int main(){

	//Variabili utility
	int i;
	pid_t pid;
	key_t key_shmem;
	key_t key_sem;
	key_t key_queue;
	int id_sem;
	int id_shm;
	int id_queue;
	LettScritt* ls;
	int conteggio_master = NUMERO_LETTORI * NUMERO_LETTURE;
	
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
						
			printf(" --- SISTEMA --- Attach alla shmem avvenuto con successo \n");


		//Sem
		id_sem = semget(key_sem,4,IPC_CREAT|0664);
		
			//Check id sem
			if (id_sem < 0 ){
				
				perror("Errore semget\n");
				exit(1);
			
			}
			
			//Inizializzazione semafori
			semctl(id_sem,SYNCH,SETVAL,1);
			semctl(id_sem,MUTEXL,SETVAL,1);
			semctl(id_sem,MUTEXS,SETVAL,1);
			semctl(id_sem,MUTEX,SETVAL,1);
			
			printf(" --- SISTEMA --- Inizializzazione semafori avvenuta con successo \n");
			
		//Queue
		id_queue = msgget(key_queue,IPC_CREAT|0664);
		
			//Check id queue
			if (id_queue < 0 ){
				
				perror("Errore msgget\n");
				exit(1);
			
			}
			
			printf(" -- SISTEMA -- Inizializzazione coda avvenuta con successo \n");

	//Generazione processi figli
	
		//Lettori
		for (i=0;i<NUMERO_LETTORI;i++){
		
			pid = fork();
			
			if (pid == 0){
			
				srand(time(NULL)*i);
			
				execv("./lettore",NULL);
			
				//Non dovrei essere qui
				perror("/////NON DOVRESTI ESSERE QUI/////\n");
				exit(1);
				
			}
		
		}
		
		
		//Scrittori
		for (i=0;i<NUMERO_SCRITTORI;i++){
		
			pid = fork();
			
			if (pid == 0){
			
				srand(time(NULL)*i);
			
				execv("./scrittore",NULL);
				
				//Non dovrei essere qui
				perror("/////NON DOVRESTI ESSERE QUI/////\n");
				exit(1);
				
			}
		
		}
		
	//Lavoro del master
	for (i=0;i<conteggio_master;i++){
	
		message msg;
		
		msgrcv(id_queue,&msg,sizeof(message)-sizeof(long),MSG_TYPE,0);
		
		printf(" [MASTER] Ricevuto il messaggio %c %c. Go next. \n",msg.char1,msg.char2);
		
	}
	
	//Attesa terminazione dei processi figli
	for (i=0;i<NUMERO_LETTORI+NUMERO_SCRITTORI;i++){
		wait(NULL);
		printf(" --- Sistema --- Processo %d terminato.\n",i+1);
	}
	
	//Cleanup shared memory e semafori e coda
	semctl(id_sem,0,IPC_RMID);
	shmctl(id_shm,IPC_RMID,0);
	msgctl(id_queue,IPC_RMID,0);
	
	printf(" --- Sistema --- Risorse deallocate\n");
		

	return 0;

}
