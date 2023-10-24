#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

#include "prodcons.h"


void produttore_1(MonitorPC * m)
{
	srand(getpid());
	printf(" Produttore di tipo 1 avviato\n");

	for(int i=0; i<4; i++) {

		int valore = rand() % 10;

		/* TBD: chiamare il metodo di produzione tipo 1 */
		produci_tipo_1(m,valore);
		printf(" Il produttore 1 ha effettuato la produzione numero %d\n",i+1);
		sleep(1);
	}

}


void produttore_2(MonitorPC * m)
{
    srand(getpid());
    printf(" Produttore di tipo 2 avviato\n");

	for(int i=0; i<4; i++) {

		int valore = rand() % 10;

		/* TBD: chiamare il metodo di produzione tipo 2 */
		produci_tipo_2(m,valore);
		printf(" Il produttore 2 ha effettuato la produzione numero %d\n",i+1);
		sleep(1);
	}

}


void consumatore_1(MonitorPC * m)
{
	printf(" Consumatore di tipo 1 avviato\n");

	for(int i=0; i<8; i++) {

		int valore;

		/* TBD: chiamare il metodo di consumazione tipo 1 */
		consuma_tipo_1(m,&valore);
		printf(" Il consumatore 1 ha effettuato la consumazione numero %d\n",i+1);
		sleep(1);
	}

}


void consumatore_2(MonitorPC * m)
{

	printf(" Consumatore di tipo 2 avviato\n");
	
	for(int i=0; i<8; i++) {

		int valore;

		/* TBD: chiamare il metodo di consumazione tipo 1 */
		consuma_tipo_2(m,&valore);
		printf(" Il consumatore 2 ha effettuato la consumazione numero %d\n",i+1);
		sleep(1);
	}
}

int main(int argc, char *argv[])
{
	int i;
	pid_t pid;

	/* TBD: creare e inizializzare l'oggetto monitor (metodo "inizializza") */
    int id_shm = shmget(IPC_PRIVATE,sizeof(MonitorPC),IPC_CREAT|0664);
    MonitorPC* m = (MonitorPC*) shmat(id_shm,0,0);
    
    	//Check da fare ma mi scoccio di farlo
    	
    //Inizializzo il monitor
    inizializza(m);


	for(i=0; i<2; i++) {

		/* TBD: avviare 2 produttori di tipo 1 */
		pid = fork();
		
		if (pid == 0){
		
			produttore_1(m);
			exit(0);
			
		}
		
	}

	for(i=0; i<2; i++) {

		pid = fork();
		
		if (pid == 0){
		
			/* TBD: avviare 2 produttori di tipo 2 */
			produttore_2(m);
			exit(0);
			
		}
		
	}

	/* TBD: avviare consumatore di tipo 1 */
	
	pid = fork();
	
	if (pid ==0){
		consumatore_1(m);
		exit(0);
	}
	
	/* TBD: avviare consumatore di tipo 2 */

	pid = fork();
	
	if (pid ==0){
		consumatore_2(m);
		exit(0);
	}

	for(i=0; i<2; i++) {

		/* TBD: attendere terminazione produttori di tipo 1 */
		wait(NULL);
		printf("--- Sistema --- Produttore tipo 1 NUMERO: %d terminato\n",i+1);
	}


	for(i=0; i<2; i++) {

		/* TBD: attendere terminazione produttori di tipo 2 */
		wait(NULL);
		printf("--- Sistema --- Produttore tipo 2 NUMERO: %d terminato\n",i+1);
	}

	/* TBD: attendere terminazione consumatori di tipo 1 */
	wait(NULL);
	printf("--- Sistema --- Consumatore tipo 1 terminato\n");

	/* TBD: attendere terminazione consumatori di tipo 2 */
	wait(NULL);
	printf("--- Sistema --- Consumatore tipo 2 terminato\n");

	/* TBD: rimuovere l'oggetto monitor */
	rimuovi(m);
	
	return 0;

}

