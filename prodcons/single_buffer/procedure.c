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


void Produttore(int* buffer, int sem_id){

	Wait_Sem(sem_id,SPAZIO_DISPONIBILE);
	sleep(2);
	*buffer = rand() % 100;
	printf("[PRODUCER] Prodotto %d\n",*buffer);
	Signal_Sem(sem_id,MESSAGGIO_DISPONIBILE);
	
}

void Consumatore(int* buffer, int sem_id){

	Wait_Sem(sem_id,MESSAGGIO_DISPONIBILE);
	sleep(2);
	printf("[CONSUMER] Consumato %d\n",*buffer);
	Signal_Sem(sem_id,SPAZIO_DISPONIBILE);

}
