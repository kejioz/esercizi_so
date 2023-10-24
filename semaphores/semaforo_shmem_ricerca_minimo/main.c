#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>
#include <limits.h>

#include "semafori.h"
#include "processini.h"

int main(){

	//Keys vettore e buffer
	key_t k_vett=IPC_PRIVATE;
	key_t k_buff=IPC_PRIVATE;
	
	//Descrittore per il vettore, per il buffer singolo e per il semaforo
	int vett_id;
	int buffer_id;
	int sem_id;
	
	//Puntatore alla shm vettore e shm buffer
	int *vettore;
	int *buffer;
	
	//Faccio shmget sia per il buffer che per il vettore
	vett_id = shmget(k_vett,sizeof(int)*10000,IPC_CREAT|0664);
	
	if (vett_id < 0){
	
		printf("Errore shmget vettore");
		exit(1);
	
	}
	
	buffer_id = shmget(k_buff,sizeof(int),IPC_CREAT|0664);
	
	if (buffer_id < 0){
		
		printf("Errore shmget buffer");
		exit(1);
	
	}
	
	//Inizializzo il semaforo chiamando la funzione
	sem_id = inizializza_semafori();
	
	//Faccio l'attach al buffer ed al vettore
	vettore = shmat(vett_id,0,1); //Metto 1 come terzo parametro siccome read only
	buffer = shmat(buffer_id,0,0); //Metto 0 invece siccome non readonly
	
	//Check sul ritorno di vettore e buffer. Ricorda shmctl ritorna void*
	if (vettore == (void*) -1){
	
		printf("Errore nell'inizializzazione del puntatore al vettore\n");
		exit(1);
		
	}
	
	if (buffer ==(void*) -1){
	
		printf("Errore nell'inizializzazione del puntatore al buffer\n");
		exit(1);
	
	}
	
	printf("Il buffer si trova a %p ed ha id descrittore: %d \n",buffer,buffer_id);
	printf("Il vettore si trova a %p ed ha id descrittore: %d \n",vettore,vett_id);
	
	//Inizializzo tutto il vattore a valori randomici non negativi
	int i;
	srand(time(NULL));
	for (i=0;i<NUM_ELEMENTI;i++){
		int val = rand()%INT_MAX;
		*(vettore+i)= (val);
	}
	
	//Inizializzo il valore nel buffer a INT_MAX
	*buffer = INT_MAX;
	
	//A questo punto posso creare i processi figli
	for (i=0;i<10;i++){
	
		//Forko. Copia di un processo con stessa heap, stack, globalv, u-area, textarea ma nuovo pid
		pid_t pid = fork();
		
		//Se c'è errore
		if ( pid < 0){
		
			printf("Errore nel fork");
			exit(1);
			
		}
		
		//Se sono il figlio
		if ( pid == 0 ){
			
			//Chiamata a funzione
			figlio(vettore,buffer,sem_id,i*1000,1000);
			
			//Esco
			exit(0);
	
		}
	
	}
	
	//Se sono il padre
	padre(buffer,sem_id);
	
	//Dellocazione risorse IPC
	shmctl(vett_id,IPC_RMID,0);
	shmctl(buffer_id,IPC_RMID,0);
	semctl(sem_id,0,IPC_RMID);
	
	return 0;

}
