#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "header.h"

int main(){

	//Avvio il processo e creo un messaggio dove il msg type (long) è il processo
	printf("Processo P1 avviato\n");
	struct msg_calc m_p1;
	m_p1.processo = P1;
	
	//Richiedo la key per la coda con ftok. Deve essere la stessa coda per tutti i processi
	key_t queue = ftok(FTOK_PATH_Q,FTOK_CHAR_Q);
	
	//Richiedo il descrittore della coda
	int id_queue = msgget(queue,IPC_CREAT | 0664);
	
	//Se nn a buon fine
	
	if(id_queue < 0) {
		perror("Msgget fallita");
		exit(1);
	}

	srand(time(NULL));
	
	int i;
	
	for (i=0;i<11;i++){
	
		//Creo i messaggi mettendo ogni volta nel messaggio un float
		m_p1.numero = generaFloat (0,10);
		printf("P1: invio messaggio <%lu,%f>\n",m_p1.processo,m_p1.numero);
		
		//Invio il messaggio
		msgsnd(id_queue,(void *)&m_p1, sizeof(struct msg_calc)-sizeof(long),0);
		sleep(1);
	
	}

	return 0;

}
