#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include "header.h"


int main(){
    
     //TODO: completare richiesta shared memory ed inizializzazione
     key_t c_mem = IPC_PRIVATE;
     Info_Volo *volo1, *volo2;
 
     // ...
     int id_volo1 = shmget(c_mem,sizeof(Info_Volo),IPC_CREAT|0664);
     
     if (id_volo1 < 0){
     	perror("Errore get\n");
     	exit(1);
     }
     
     int id_volo2 = shmget(c_mem,sizeof(Info_Volo),IPC_CREAT|0664);
     
     if (id_volo2 < 0){
     	perror("Errore get\n");
     	exit(1);
     }
     
     volo1 = (Info_Volo*) shmat(id_volo1,0,0);
     volo2 = (Info_Volo*) shmat(id_volo2,0,0);
     
     if (volo1 == (void*) -1 || volo1 == (void*) -1){
     	perror("Errore attach\n");
     	exit(1);
     }
     
     //TODO inizializzare id_volo pari a 1 per volo1 e pari a 2 per volo2
     srand(time(NULL));
     volo1->id_volo = 1;
     volo1->numero_lettori = 0;
     volo1->gate = rand()%235;
     volo2->id_volo = 2;
     volo2->numero_lettori = 0;
     volo2->gate = rand()%235;

     //TODO: compleare richiesta semafori ed inizializzazione
     key_t c_sem= IPC_PRIVATE;
     
     // ...
     int id_sem = semget(c_sem,4,IPC_CREAT|0664);
     
     if (id_sem < 0){
     	perror("Errore get\n");
     	exit(1);
     }
     
     semctl(id_sem,MUTEXL1,SETVAL,1);
	 semctl(id_sem,SYNCH1,SETVAL,1);
	 semctl(id_sem,MUTEXL2,SETVAL,1);
	 semctl(id_sem,SYNCH2,SETVAL,1);
	 
     //TODO: compleatare richiesta coda messaggi
     key_t c_coda= ftok(FTOK_PATH,FTOK_CHAR_CODA);
     // ...
     int id_coda = msgget(c_coda,IPC_CREAT|0664);
     
     int i;
     pid_t pid;
        
     //TODO generazione tramite fork di scrittore e 4 lettori, 2 per il volo 1 e 2 per il volo 2
     
     //Scrittore
   	 pid = fork();
   	 
   	 if (pid == 0){
   	 	srand(time(NULL)*getpid());
   	 	Scrittore(volo1,volo2,id_sem);
   	 	exit(0);
   	 	
   	 }
   	 
   	 for (i=0;i<NUM_LETTORI;i++){
   	 
   	 	pid = fork();
   	 	
   	 	if (pid == 0){
   	 		srand(time(NULL)*getpid());
   	 		if (i%2 ==0){
   	 		
   	 			//Volo 1
   	 			Lettore(volo1,id_coda,id_sem);
   	 			
   	 		}else{
   	 		
   	 			//Volo 2
   	 			Lettore(volo2,id_coda,id_sem);
   	 			
   	 		}
   	 		
   	 		exit(0);
   	 		
   	 	}
   	 	
   	 }
   	 
   	 
     //TODO generazione tramite fork/exec del visualizzatore
     //Visualizzatore
     
     pid = fork();
     
     if (pid == 0){
     	char* params[] = {NULL};
     	execv("./visualizzatore",params);
     	printf("Non dovresti essere qui\n");
     	exit(1);
     
     }

     //TODO: attesa terminazione dei "numprocessi"  figli
     int numprocessi = NUM_LETTORI+2;
     
     for (i=0;i<numprocessi;i++){
     	wait(NULL);
     	printf(" --- Sistema --- Figlio %d terminato.\n",i+1);
     }
    

     //TODO: Deallocazione risorse
     shmctl(id_volo1,IPC_RMID,0);
     shmctl(id_volo2,IPC_RMID,0);
     semctl(id_sem,0,IPC_RMID);
     msgctl(id_coda,IPC_RMID,0);
     printf(" ///RISORSE DELLOCATE///\n");
	
     return 0;
}
