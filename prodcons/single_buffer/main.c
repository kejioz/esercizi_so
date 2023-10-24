#include "procedure.h"
#include "semafori.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <time.h>

int main(){

	srand(time(NULL));
	//Genero le keys per il semaforo e per la shmem di dim sizeofint
	key_t k_sh = IPC_PRIVATE;
	key_t k_sem = IPC_PRIVATE;
	
	//Faccio richiesta dei descrittori
	int ds_sem;
	int ds_shm;
	
	//Prendo descrittore shared memory
	ds_shm = shmget(k_sh,sizeof(int),IPC_CREAT|0664);
	
	//Controllo che la creazione di shmem sia andata a buon fine
	if (ds_shm < 0){
	
		printf("Errore shmget\n");
		exit(1);
	
	}	
	
	//Assegno il puntatore alla shm al mio processo
	int*p;
	p = shmat(ds_shm,0,0);
	
	//Controllo che l'attach sia andato a buon fine ricorda void*
	if (p == (void*) -1){
	
		printf("Errore nell'attach della shared memory\n");
		exit(1);
	
	}
	
	//Inizializzo a 0 il buffer
	*p = 0;
	
	//Prendo descrittore del semaforo
	ds_sem = semget(k_sem,2,IPC_CREAT|0664); //Mi servono due semafori
	
	//Controllo che la creazione di sem sia andata a buon fine
	if (ds_sem < 0){
	
		printf("Errore semget\n");
		exit(1);
	
	}	
	
	//Inizializzo i due semafori COOPERAZIONE
	semctl(ds_sem,SPAZIO_DISPONIBILE,SETVAL,1);
	semctl(ds_sem,MESSAGGIO_DISPONIBILE,SETVAL,0);
	printf("Semafori inizializzati\n");
	
	//Creo i due processi
	
	int pid = fork();
	
	//Figlio
	if (pid == 0){
	
		//Consumatore
		printf("Inizio consumazione\n");
		Consumatore(p,ds_sem);
		exit(1);
	
	}
	
	pid = fork();
	if (pid == 0){
	
		//Produttore
		printf("Inizio produzione\n");
		Produttore(p,ds_sem);
		exit(1);
	
	}
	
	//Wait del padre
	wait(NULL);
	printf("Padre : primo figlio terminato\n");
	wait(NULL);
	printf("Padre : secondo figlio terminato\n");
	
	//Cleanup risorse
	shmctl(ds_shm,IPC_RMID,NULL);
	printf("Shared memory pulita\n");
	semctl(ds_sem,0,IPC_RMID);
	printf("Semaforo pulito\n");
	
	return 0;

}
