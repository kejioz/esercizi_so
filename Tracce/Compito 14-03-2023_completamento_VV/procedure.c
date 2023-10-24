#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include "header.h"
#include <time.h>


/*******PROCEDURE PER I SEMAFORI******/


void Wait_Sem(int id_sem, int numsem)     {
       struct sembuf sem_buf;
       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=-1;
       semop(id_sem,&sem_buf,1);   //semaforo rosso
}


  void Signal_Sem (int id_sem,int numsem)     {
       struct sembuf sem_buf;
       sem_buf.sem_num=numsem;
       sem_buf.sem_flg=0;
       sem_buf.sem_op=1;
       semop(id_sem,&sem_buf,1);   //semaforo verde
}


/***********************************************/

void Lettore(Info_Volo *volo, int coda, int id_sem){
	int i;
	int mut,synch;
	if (volo->id_volo == 1){
		mut = MUTEXL1;
		synch = SYNCH1;
	}else if (volo->id_volo == 2){
		mut = MUTEXL2;
		synch  = SYNCH2;
	}
	
        for (i=0; i<NUM_OPERAZIONI; i++) {

				srand(time(NULL)*getpid()*i);
                //TODO: accesso alla risorsa secondo soluzione lettori/scrittori, starvation scrittori
                
                //Per prima cosa voglio fare la wait sul semaforo numerolettori
                Wait_Sem(id_sem,mut);
                
                	//Posso aumentare il numero di lettori
                	volo->numero_lettori= (volo->numero_lettori) +1;
                	printf("[LETTORE %d] Numero lettori aumentato; attuale : %d\n",volo->id_volo,volo->numero_lettori);
                	if (volo->numero_lettori == 1){
                	
                		//Se sono il primo lettore faccio aspettare gli scrittori
                		Wait_Sem(id_sem,synch);
                	
                	}
                	
               	//A questo punto segnalo che ho fatto con numero lettori
               	Signal_Sem(id_sem,mut);
                
                //TODO: creazione e invio messaggio al visualizzatore
				
				//Effettuo la lettura
				msg messaggio;
				messaggio.volo = (long) volo->id_volo;
				messaggio.gate = volo->gate;
				
				//Posso inviare il messaggio
				msgsnd(coda,&messaggio,sizeof(msg)-sizeof(long),0);
				printf("[LETTORE %d] Messaggio per volo %u inviato con successo. Gate: %d\n",volo->id_volo,volo->id_volo,volo->gate);
               
                sleep(1);

               //TODO: rilascio risorsa secondo soluzione lettori/scrittori, starvation scrittori
               	//Devo modificare numero lettori
               	Wait_Sem(id_sem,mut);
               		
               		//Decremento
               		volo->numero_lettori= (volo->numero_lettori) -1;
               		printf("[LETTORE %d] Numero lettori decrementato; attuale : %d\n",volo->id_volo,volo->numero_lettori);
               		
               		//Se sono l'ultimo lettore sveglio gli scrittori
               		if (volo->numero_lettori == 0){
               		
               			printf("[LETTORE %d] Sono l'ultimo lettore, sveglio gli scrittori\n",volo->id_volo);
               			Signal_Sem(id_sem,synch);
               		
               		}
               		
               	Signal_Sem(id_sem,mut);

        }

}

void Scrittore(Info_Volo* volo1, Info_Volo* volo2,int id_sem) {
        int i;
		
        for (i=0; i<NUM_OPERAZIONI; i++) {
        
				srand(time(NULL)*getpid()*i);
                //TODO: accesso alle risorse secondo soluzione lettori/scrittori, starvation scrittori 
                //Aspetto il mio turno su synch...
                Wait_Sem(id_sem,SYNCH1);
                Wait_Sem(id_sem,SYNCH2);
                	
                //TODO: gate del volo 2 = gate del volo 1
                volo2->gate = volo1->gate;
                //TODO: gate del volo 1 = valore casuale da 1 a 100 
                volo1->gate = (rand()%100)+1;
                printf("[SCRITTORE] Posso scrivere finalmente.Il gate del volo2 è ora %d ed il gate del volo1 è ora %d\n",volo2->gate,volo1->gate);
                sleep(1);
                //TODO: rilascio  risorse secondo soluzione lettori/scrittori, starvation scrittori 
                Signal_Sem(id_sem,SYNCH1);
                Signal_Sem(id_sem,SYNCH2);
        }

}


