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

	//Genero le keys per il semaforo e per la shmem di dim struct codaCircolare
	key_t k_sh = IPC_PRIVATE;
	key_t k_sem = IPC_PRIVATE;
	
	//Faccio richiesta dei descrittori
	int ds_sem;
	int ds_shm;
	
	//Prendo descrittore shared memory
	ds_shm = shmget(k_sh,sizeof(struct vettoreStato),IPC_CREAT|0664);
	
	//Controllo che la creazione di shmem sia andata a buon fine
	if (ds_shm < 0){
	
		printf("Errore shmget\n");
		exit(1);
	
	}	
	
	//Assegno il puntatore alla shm al mio processo. Nota che è un puntatore a struct
	struct vettoreStato* p;
	p = (struct vettoreStato*) shmat(ds_shm,0,0);
	
	//Controllo che l'attach sia andato a buon fine ricorda void*
	if (p == (void*) -1){
	
		printf("Errore nell'attach della shared memory\n");
		exit(1);
	
	}
	
	
	//Prendo descrittore del semaforo
	ds_sem = semget(k_sem,4,IPC_CREAT|0664); //Mi servono due semafori
	
	//Controllo che la creazione di sem sia andata a buon fine
	if (ds_sem < 0){
	
		printf("Errore semget\n");
		exit(1);
	
	}	
	
	//Inizializzo i due semafori COOPERAZIONE
	semctl(ds_sem,SPAZIO_DISPONIBILE,SETVAL,DIM_BUFFER);
	semctl(ds_sem,MESSAGGIO_DISPONIBILE,SETVAL,0);
	//Inizializzo i due semafori COMPETIZIONE
	semctl(ds_sem,MUTEX_C,SETVAL,1);
	semctl(ds_sem,MUTEX_P,SETVAL,1);
	
	printf("Semafori inizializzati\n");
	
	//Creo n processi
	
	for (int i=0;i<NUM_CONSUMATORI+NUM_PRODUTTORI;i++){
	
		int pid = fork();
		
		//Se sono il figlio
		if (pid == 0){
		
			if (i%2==0){
			
				//Consumatore
				printf("Inizio figlio consumatore %d\n",i);
				srand(getpid()*time(NULL));
				Consumatore(p,ds_sem);
				exit(0);
			
			}
			
			else if (i%2!=0){
			
				//Produttore
				sleep(1);
				printf("Inizio figlio produttore %d\n",i);
				srand(getpid()*time(NULL));
				Produttore(p,ds_sem);
				exit(0);
				
			}
		
		}
	
	}
	
	
	for (int i=0;i<NUM_PRODUTTORI+NUM_CONSUMATORI;i++){
	
		wait(NULL);
		
		if (i==NUM_PRODUTTORI+NUM_CONSUMATORI){
		
			printf("Tutti i figli hanno terminato l'esecuzione\n");
			
		}
		
	}
	

	
	//Cleanup risorse
	shmctl(ds_shm,IPC_RMID,NULL);
	printf("Shared memory pulita\n");
	semctl(ds_sem,0,IPC_RMID);
	printf("Semafori puliti\n");
	
	return 0;

}
