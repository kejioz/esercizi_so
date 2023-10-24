#include "procedure.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(){

	//Chiavi per shm e per semaforo
	key_t key_shm = ftok(PATH,ID);
	key_t key_sem = ftok(PATH,ID_SEM);
	
	//Prendo i descrittori di shmemory e semaforo
	int id_shm = shmget(key_shm,sizeof(esame_t),IPC_CREAT|0664);
	int id_sem = semget(key_sem,3,IPC_CREAT|0664);
	
	if (id_shm < 0 || id_sem <0){
	
		printf("Errore semget / shmget \n");
		exit(1);
	
	}
	
	//Puntatore alla shared memory ed inizializzazione semafori
	esame_t* esame = (esame_t*) shmat(id_shm,0,0);
	esame->prossimo_appello[0]='\0';
	esame->numero_prenotati=0;
	esame->numero_lettori=0;
	if (esame == (void*) -1){
	
		printf("Errore shmat\n");
		exit(1);
		
	}

	//Inizializzazione semafori
	semctl(id_sem,MUTEX,SETVAL,1);
	semctl(id_sem,APPELLO,SETVAL,1);
	semctl(id_sem,PRENOTATI,SETVAL,1);

	
	//Generazione docente e 10 studenti
	
	pid_t pid = fork();
	char* args[] = {NULL};
	
	if (pid == 0){
	
		srand(getpid()*time(NULL));
		//Docente
		execv("./docente",args);
		
		//Se sono qui c'è un errore
		printf("Non dovrei eseguire questo codice\n");
		exit(1);
	
	}
	
	int i;
	for (i=0;i<10;i++){
	
		pid = fork();
		sleep(1);
		if (pid == 0){
		
			srand(getpid()*time(NULL));
			//Studenti
			execv("./studente",args);
			
			//Se sono qui c'è un errore
			printf("Non dovrei eseguire questo codice\n");
			exit(1);
			
		}
	
	}
	
	//Attendo la terminazione dei figli
	for (i=0;i<11;i++){
		wait(NULL);
	}
	
	//Pulisco risorse
	shmctl(id_shm,IPC_RMID,0);
	semctl(id_shm,0,IPC_RMID);


	return 0;

}
