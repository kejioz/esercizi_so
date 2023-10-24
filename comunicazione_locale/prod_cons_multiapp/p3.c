#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#include "header.h"


int main() {

	//Creo la struct per il mess
	printf("Processo P3 avviato\n");
	struct msg_calc m_r;
	
	//Creo l'array per storare i valori
	float media_cum[2];
	media_cum[1]=0;
	media_cum[0]=0;
	
	//Prendo la key
	key_t queue = ftok(FTOK_PATH_Q,FTOK_CHAR_Q);
	
	//Prendo il descrittore della coda
	int id_queue = msgget(queue,IPC_CREAT|0664);
	
	printf("ID QUEUE: %d\n",id_queue);
	
	//In caso di errore
	if(id_queue < 0) {
		perror("Msgget fallita");
		exit(1);
	}
	
	int i;
	
	for (i = 0;i<22;i++){
	
		//Ricevo sulla coda e storo nel messaggio. Ricorda il flag 0 è per ricevere tt i mess
		msgrcv(id_queue, (void *)&m_r,sizeof(struct msg_calc)-sizeof(long),0,0);
		
		printf("Ricevuto messaggio dal processo <%lu> ,con val <%f>\n",m_r.processo,m_r.numero);
		
		if(m_r.processo == P1){
		
			media_cum[0]+= m_r.numero/11;
		
		}
	
		else if (m_r.processo == P2){
		
			media_cum[1]+= m_r.numero/11;
		
		}
	
		else{
		
			printf("Processo non riconosciuto\n");
		
		}
	
	}

	for (i = 0;i < 2;i++){
	
		printf("<Media %d = %f>\n",i+1,media_cum[i]);
	
	}
	
	
	return 0;	
	
}
