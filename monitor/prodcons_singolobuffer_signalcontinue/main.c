#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#include "procedure.h"

#define NUM_PRODUTTORI 1
#define NUM_CONSUMATORI 1

int main(){

    //CHIAVE SHM DOVE ALLOCHERÒ LA MIA STRUTTURA PRODCONS
    key_t chiave_shm = IPC_PRIVATE;

    //Prendo il descrittore della shm
    int id_shm = shmget (chiave_shm, sizeof(struct ProdCons),IPC_CREAT|0664);

    //Check
    if( id_shm < 0 ) {
        perror("errore shm");
        exit(1);
	}

	//Puntatore alla shm
	struct ProdCons* pc = shmat(id_shm,NULL,0);

    //CHeck
    if (pc == (void*)-1){
        perror("errore shmat");
		exit(1);
    }

    //Inizializzo il monitor all'interno della shmem e gli dico quante cond variables
    init_monitor(&(pc->m),2);

    //Setto le condvar
    pc->buffer_libero = 1;
    pc->buffer_occupato = 0;

    //Variabili per figli
    int j;
    pid_t pid;

    //CONSUMATORE
    for (j=0;j<NUM_CONSUMATORI;j++){

        pid = fork();

        if (pid == 0){

            //Consumatore
            printf("Inizio consumatore\n");

            int valore = Consuma (pc);

			printf("Consumazione: %d\n", valore);

			exit(0);

        }

        else if( pid < 0 ) {
			perror("errore fork");
			exit(1);
		}

    }

    //PRODUTTORE
    for (j=0;j<NUM_PRODUTTORI;j++){

        pid = fork();

        if (pid == 0){

            //Consumatore
            printf("Inizio produzione\n");

            sleep(5);

			srand(time(NULL));

			int valore = rand() % 10;

			Produci( pc, valore);

			printf("Produzione: %d\n", valore);

			exit(0);

        }

        else if( pid < 0 ) {
			perror("errore fork");
			exit(1);
		}

    }

    //WAITO

	for(j=0; j<NUM_CONSUMATORI+NUM_PRODUTTORI; j++) {
		wait(NULL);
	}

	//PULISCO MONITOR E SHM
    remove_monitor( &(pc->m) );

	shmctl( id_shm, IPC_RMID, 0);


    return 0;

}
