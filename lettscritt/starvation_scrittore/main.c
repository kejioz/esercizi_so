#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "procedure.h"
#include "semafori.h"

int main(){

	int sem, id_shared, k, numsem, numlettori,numscrittori;
	key_t chiave = IPC_PRIVATE;
	key_t c_sem = IPC_PRIVATE;
	int status,numprocessi;
	pid_t pid;
	Buffer* buf;
	
	numlettori = 6;
	numscrittori = 6;
	int num_processi = numscrittori+numlettori;
	
	//Prendo il descrittore del buffer
	id_shared = shmget(chiave,sizeof(Buffer),IPC_CREAT|0664);
	printf("id_shared = %d\n", id_shared);
	
	//Faccio l'attach
	buf = (Buffer*) shmat(id_shared,0,0);
	
	//Inizializzo il buffer
	buf->numlettori = 0;
	buf->messaggio=0;
	
	//Richiedo due semafori e li inizializzo
	sem = semget(c_sem,2,IPC_CREAT | 0664);
	semctl(sem,MUTEXL,SETVAL,1);
	semctl(sem,SYNCH,SETVAL,1);
	
	//Genero i processi figli
	for (k=0; k<num_processi; k++) {
	
		pid = fork();
		
		if ( pid == 0 ){
			
			if (k%2 == 0){
			
				printf("Sono il figlio scrittore dal pid %d \n",getpid());
				Scrittore(sem,buf);
			
			}
			
			else{
			
				printf("Sono il figlio lettore dal pid %d \n",getpid());
				Lettore(sem,buf);
		
			}
			
			exit(0);
			
		}
	
	
	}
	
	//Attendo la terminazione dei figli
	
	for (k=0; k<num_processi; k++){
            pid=wait(&status);
            if (pid==-1)
                    perror("errore");
            else
                    printf("Figlio n.ro %d e\' morto con status= %d\n", pid, status);
    }
        
    
    //Deallocazione semafori e shmem
    shmctl(id_shared,IPC_RMID,0);
    semctl(sem,0,IPC_RMID);
    
    return 0;
      

}
