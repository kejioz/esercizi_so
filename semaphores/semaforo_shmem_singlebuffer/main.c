#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#include "semafori.h"

int main(){

	//Per prima cosa prendo le IPC key sia per i semafori, sia per la shmem
	//Posso usare IPC_PRIVATE siccome mi trovo in dei child processes, altrimenti avrei dovuto usare ftok
		//Ricorda che con ftok devi dare un path ed un id anche char ftok(pathname,id()
	key_t k_shmem = IPC_PRIVATE;
	key_t k_sem = IPC_PRIVATE;
	
	//Adesso prendo il descrittore della shmem. Gli passo la key, la size (un int) ed i flags
	int id_shmem = shmget(k_shmem,sizeof(int),IPC_CREAT|0664);
	
	//Check sul ritorno
	if (id_shmem < 0 ){
	
		printf ("Errore SHMEM");
		exit(1);
	
	}
	
	//Prendo il descrittore del SET di semafori. Gli passo la key, quanti semafori ed i flags
	int id_sem = semget(k_sem,1,IPC_CREAT|0664);
	
	//Check sul ritorno
	if (id_sem < 0){
	
		printf ("Erorre SEMGET");
		exit(1);
	
	}
	
	//Adesso faccio l'attach sulla shared memory. Devo restituire ad un puntatore l'indirizzo virtuale
	int* ptr = shmat(id_shmem,0,0); //Il secondo flag dice dammi tu la memoria, il terzo flag di readonly
	
	//Controllo se va a buon fine l'attach. Ricorda che l'attach restituisce un void* quindi devo castare il -1 a void*
	if (ptr == (void*)-1){
	
		printf("Errore SHMAT");
		exit(1);
	
	}
	
	//Adesso inizializzo il semaforo con semctl passandogli id del set, QUALE semaforo dell'array
	semctl(id_sem,0,SETVAL,1); //Faccio setval ed inizializzo ad uno
	printf("SEMAFORO %d INIZIALIZZATO AD 1\n",id_sem);
	
	//Setto il valore iniziale della shmem
	*ptr = 0;
	printf("VALORE INIZIALE DELLA SHMEM %p INIZIALIZZATO A %d\n",ptr,*ptr);
	
	//Creo due processi
	int i;
	for (i=0;i<2;i++){
	
		pid_t pid = fork();
		
		//Se non va a buon fine
		if (pid < 0){
		
			printf("Errore FORK");
			exit(1);
		
		}
	
		//Se sono il figlio
		else if (pid == 0){
		
			printf("PROCESSO[%d] con PID %d CREATO. PADRE:%d \n",i,getpid(),getppid());
			
			//Codice da eseguire
			int j;
			for (j=0;j<100;j++){
			
				//Entro nella sezione critica
				Wait_Sem(id_sem,0); //Gli passo il descrittore del set e quale semaforo nel set
				
					//AUMENTO DI UNO
					(*ptr)++;
				
				//Esco dalla sezione critica
				Signal_Sem(id_sem,0); //Gli passo il descrittore del set e quale semaforo nel set
				
			
			}
			
			//Termino il processo con successo
			
			exit(0);
		
		}
		
		//Se sono il padre non devo fare niente
		
	}
	
	//Nota che qua posso continuare tranquillamente senza checkare se sono il padre perchè ho già fatto exit dei child
	
	//Faccio la wait (sono il padre)
	for (i=0;i<2;i++){
		wait(NULL);
	}
	
	printf("IL VALORE FINALE CONTENUTO NELLA SHMEM %p È %d\n",ptr,*ptr);
	
	//Ricordo di fare il cleanup sia della shmem che del semaforo //RICORDA CHE IL 3 PARAMETRO È BUFFER (i/o)
	shmctl(id_shmem,IPC_RMID,NULL);
	semctl(id_sem,0,IPC_RMID);

	return 0;
	
}
