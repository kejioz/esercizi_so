#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

#include "header.h"


int main(){
	pid_t pid_ex,pid;
	int st,i;
	int shmid,semid;
	BufferCircolare* buf;

	key_t key_shm,key_sem;
	key_shm = IPC_PRIVATE;
	key_sem = IPC_PRIVATE;

	shmid = shmget(key_shm,sizeof(BufferCircolare),IPC_CREAT | 0664);
	semid = semget(key_sem, 6,IPC_CREAT | 0664);
	printf("[MASTER] - Shmid : %d, Semid: %d\n",shmid,semid);

	semctl(semid,SPAZIO_DISP,SETVAL,SIZE);
	semctl(semid,MUTEXP,SETVAL,1);
	semctl(semid,MESS_DISP_1,SETVAL,0);
	semctl(semid,MESS_DISP_2,SETVAL,0);
	semctl(semid,MESS_DISP_3,SETVAL,0);
	semctl(semid,MUTEXC,SETVAL,1);

	buf = (BufferCircolare*) shmat(shmid,0,0);
	
	//inizializzazione della struttura
	for(int i=0; i<SIZE;i++)
		buf->stati[i] = LIBERO;

	srand(time(NULL));
	int val;

	for(i = 0; i < NCONS+NPROD; i++){
		pid = fork();
		if(pid == 0){
			if(i < 3){
				//Instanzia consumatore
				consuma(buf, i+1, semid);
			}else{
				//Instanzia produttore
				produci(buf, i%3 +1 , semid);
			}
			_exit(0);			
		}
		val =  (1+rand()%3);
		sleep(val);

	}
	
	for(i = 0; i < NCONS+NPROD ; i++){
		pid_ex = wait(&st);
		printf("[MASTER] - Il processo %d ha terminato l'esecuzione\n",pid_ex);			
	}
	shmctl(shmid,IPC_RMID,0);
	semctl(semid,0,IPC_RMID);
	return 0;
}
