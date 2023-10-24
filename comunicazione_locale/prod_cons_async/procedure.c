#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include "header.h"
#include <time.h>

void Produttore(int queue, char * text){

	Messaggio m;
	
	//Costruisco il messaggio da trasmettere
	
	m.tipo=MESSAGGIO; //Tipo del messaggio
	strcpy(m.mess,text); //Messaggio effettivo
	
	//Invio il messaggio NOTA CHE MSGSND È ASINCRONA TRANNE QUANDO LA MAILBOX È PIENA
	//Specifico l'id della coda, l'indirizzo della struct, la SIZE
	//ED IL FLAG NO_WAIT che se la mailbox è piena ritorna -1 e non accoda. Se il flag è 0 blocca
	msgsnd(queue,(void*)&m, sizeof(Messaggio)-sizeof(long),IPC_NOWAIT); //Ritorna 0 se va tt ok
	
	printf("Messaggio inviato con successo!: <%s>\n",m.mess);

}

void Consumatore(int queue){

	Messaggio m;
	
	//DEVO RICEVERE IL MESSAGGIO!
	//Primo campo id coda, secondo campo indirizzo nel quale ricevere, terzo size, QUARTO TIPO!!!
	//QUARTO FLAG SE 0 PRENDE IL PRIMO MESS, SE >0 PRENDE QUEL TIPO, SE <0 TUTTI QUELLI <=
	//QUINTO FLAG SE 0 RICEZIONE BLOCCANTE, SE NOWAIT NON BLOCCANTE
	msgrcv(queue,(void*)&m, sizeof(Messaggio)-sizeof(long),MESSAGGIO,0); //Forse nn è 0 qua
	
	printf("Messaggio ricevuto!: <%s>\n",m.mess);
	printMsgInfo(queue);

}

void printMsgInfo(int queue){

	//Questa è una struct per le info
	struct msqid_ds mid;
	
	//Faccio una msgctl in cui richiedo stat e copio nella mia struct
	msgctl(queue,IPC_STAT,&mid);
	
	char *time_sender = ctime(&mid.msg_stime);
	char *time_receiver = ctime(&mid.msg_rtime);
	char * time_ctime = ctime(&mid.msg_ctime);
	
	
		printf("Time Sender: %sTime Receiver: %sTime Ctime: %s",time_sender,time_receiver,time_ctime);
	printf("Messages Number: %lu\n",mid.msg_qnum);

}
