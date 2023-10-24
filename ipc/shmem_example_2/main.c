#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(){

	//Getto la key che è private, quindi condivisa dal padre solo con i figi
	key_t shm_key = IPC_PRIVATE;
	
	//Getto il descrittore della shm
	int ds_shm = shmget(shm_key, sizeof(int), IPC_CREAT|0664);
	
	//Se ho errore
	if (ds_shm<0){
		printf("Errore shmget!");
		exit(1);
	}
	
	//Faccio l'attach del processo : piazzo il riferimento alla shm nello spazio di indirizzamento del mio processo
	int* p = shmat(ds_shm,NULL,0);
	
	if (p==(void*)-1){
	
		printf("Errore shmat");
		exit(1);
		
	}
	
	printf("Dettagli della shared memory creata.. : \n");
	printf("Chiave IPC shm: %d\n",shm_key);
	printf("Descrittore IPC shm: %d\n",ds_shm);
	printf("Indirizzo dopo l'attach: %p\n", p);
	
	//Adesso forko. Nota che il processo figlio ha tutte le info del padre
	key_t pid = fork();
	
	//Se figlio
	if (pid==0){
		
		*p = 123;
		exit(0);
	
	}
	
	else if (pid>0){
	
		wait(NULL);
		printf("Contenuto SHM: %d\n",*p);
	
	}
	
	//Pulisco la SHMEM
	
	shmctl(ds_shm,IPC_RMID,NULL);

	return 0;
	
	
}
