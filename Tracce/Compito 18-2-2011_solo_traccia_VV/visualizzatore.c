#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/types.h>
int main(){
	
	//Variabili utility
	int i, pid;
	
	//Keys
	key_t key_disponibilità = ftok(FTOK_PATH,FTOK_DISP);
	key_t key_posti = ftok(FTOK_PATH,FTOK_POSTI);
	key_t key_sem = ftok(FTOK_PATH,FTOK_SEM);
	
	//Descrittori
	int id_disp = shmget(key_disponibilità,sizeof(int),IPC_CREAT|0664);
	int id_posti = shmget(key_posti,sizeof(structposti),IPC_CREAT|0664);
	int id_sem = semget(key_sem,3,IPC_CREAT|0664);
	
	//Puntatore a posti, puntatore a disponibilità
	int* disponibilita = (int*) shmat(id_disp,0,0);
	structposti* p = (structposti*) shmat(id_posti,0,0);
	
		visualizzatore(id_sem,p,disponibilita);
		exit(0);
	
	//Rimozione posti e disponibilità e sem
	semctl(id_sem,0,IPC_RMID);
	shmctl(id_posti,IPC_RMID,0);
	shmctl(id_disp,IPC_RMID,0);
	

	return 0;

}
