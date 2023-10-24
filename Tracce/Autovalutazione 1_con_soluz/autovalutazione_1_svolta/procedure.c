			/*----IMPLEMENTAZIONE DELLE PROCEDURE----*/


#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <math.h>

//Procedure per la sincronizzazione sui semafori

void Wait_Sem(int id_sem, int numsem)     {
	struct sembuf sem_buf;
	sem_buf.sem_num = numsem;
	sem_buf.sem_op = -1;
	sem_buf.sem_flg = 0;
	semop(id_sem,&sem_buf,1);
}


void Signal_Sem (int id_sem,int numsem)     {
	struct sembuf sem_buf;
	sem_buf.sem_num = numsem;
	sem_buf.sem_op = 1;
	sem_buf.sem_flg = 0;
	semop(id_sem,&sem_buf,1);

}

// Consumo
void consuma(BufferCircolare* buf, int chiave, int semid){
	printf("[Consumatore %d] iniziato con chiave %d \n",getpid(), chiave);
	int sem;
	int value;
	int index;
	if( chiave== 1){
		sem = MESS_DISP_1;
	} else if (chiave == 2){
		sem = MESS_DISP_2;
	} else {
		sem = MESS_DISP_3;
	}
	for(int i=0; i<3; i++){
		Wait_Sem(semid,sem);
		printf("[Consumatore %d] entrato con chiave %d \n",getpid(), chiave);
		Wait_Sem(semid,MUTEXC);
		index = 0;
		while((buf->stati[index] != OCCUPATO || buf->elementi[index].chiave != chiave) && index < SIZE){
			index++;
		}
		buf->stati[index] = IN_USO;
		Signal_Sem(semid,MUTEXC);
		printf("[Consumatore %d] consumo elemento in posizione %d\n",getpid(), index);	
		value = buf->elementi[index].valore;
		buf->stati[index] = LIBERO;
		Signal_Sem(semid,SPAZIO_DISP);
		printf("[Consumatore %d] valore letto: %d\n",getpid(), value);
	}
}

//Produzione
void produci(BufferCircolare* buf, int chiave, int semid){
	printf("[Produttore %d] iniziato con chiave %d\n",getpid(), chiave);
	int val_sleep;
	int value;
	int sem;
	int index;
	if(chiave == 1){
		sem = MESS_DISP_1;
	} else if (chiave == 2){
		sem = MESS_DISP_2;
	} else {
		sem = MESS_DISP_3;
	}

	for(int i=0; i<3; i++){
		Wait_Sem(semid,SPAZIO_DISP);
		printf("[Produttore %d] entrato con chiave %d \n",getpid(), chiave);
		Wait_Sem(semid,MUTEXP);
		index = 0;
		while(buf->stati[index] != LIBERO && index < SIZE){
			index++;
		}
		buf->stati[index] = IN_USO;
		Signal_Sem(semid,MUTEXP);
		value = rand()%20;
		val_sleep =  (1+rand()%3);
		sleep(val_sleep);
		buf->elementi[index].chiave = chiave;
		buf->elementi[index].valore = value;
		buf->stati[index] = OCCUPATO;
		printf("[Produttore %d] ho prodotto %d, con chiave %d, in posizione %d\n",getpid(),value, chiave, index);
		Signal_Sem(semid,sem);
	}
}
