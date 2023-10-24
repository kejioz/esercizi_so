#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <time.h>

#include "procedure.h"

#define NUM_LETTORI 3
#define NUM_SCRITTORI 3

#define LETTURE 3
#define SCRITTURE 2

int main() {

	int id_queue, id_shm;
	key_t chiave_shm = IPC_PRIVATE;
	key_t chiave_mbx = IPC_PRIVATE;

	id_shm = shmget( chiave_shm, sizeof(struct MonitorRisultati), IPC_CREAT|0664);
	id_queue = msgget( chiave_mbx, IPC_CREAT|0664);

	if( id_shm < 0 || id_queue < 0) {
		perror("errore creazione risorse");
		exit(1);
	}

	struct MonitorRisultati * ls = shmat( id_shm, NULL, 0);

	if( ls == (void*)-1 ) {
		perror("errore shmat");
		exit(1);
	}


	init_monitor( &(ls->m), 2 );

	ls->numero_lettori = 0;
	ls->numero_scrittori = 0;
	for(int i=0; i< N_OPS; i++)
		ls->risultati[i] = -1;

	pid_t pid;

	int j;
	for(j=0; j < N_PRINTERS ; j++) {
		pid = fork();
		if( pid == 0 ) {
			printf("Inizio printer\n");
			printer(ls);
			exit(0);
		}
		else if( pid < 0 ) {
			perror("errore fork");
			exit(1);
		}
	}

	int operazioni[3];
	operazioni[0] = SOMMA;
	operazioni[1] = PRODOTTO;
	operazioni[2] = DIVISIONE;
	
	for(j=0; j < N_CALCOLATORI ; j++) {
		pid = fork();
		if( pid == 0 ) {
			printf("Inizio calcolatore\n");
			calcolo(ls, operazioni[j], id_queue);
			exit(0);
		}
		else if( pid < 0 ) {
			perror("errore fork");
			exit(1);
		}
	}

	for(j=0; j < N_GENERATORI ; j++) {
		pid = fork();
		if( pid == 0 ) {
			printf("Inizio generatore\n");
			generazione(operazioni[j], id_queue);
			exit(0);
		}
		else if( pid < 0 ) {
			perror("errore fork");
			exit(1);
		}
		sleep(1);
	}





	for(j=0; j<N_CALCOLATORI+N_GENERATORI+N_PRINTERS; j++)
		wait(NULL);

	remove_monitor( &(ls->m) );
	shmctl( id_shm, IPC_RMID, 0);
	msgctl(id_queue, IPC_RMID, 0);

}





