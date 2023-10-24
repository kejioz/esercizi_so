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

//STATICHE -> NON È NECESSARIA LA VISIBILITÀ FUORI DAL MODULO. FACCIO USO DI 2 CODE AUSILIARIE
//SONO STATICHE PERCHÈ LE VOGLIO TENERE VIVE FIN QUANDO NON FINISCO

static int queue1;
static int queue2;

void initServiceQueues(){

	queue1 = msgget(IPC_PRIVATE,IPC_CREAT|0664);
	queue2 = msgget(IPC_PRIVATE,IPC_CREAT|0664);
	
}

void removeServiceQueues(){

	//Ricorda il terzo parametro è il buffer, qua posso mettere a 0
	msgctl(queue1,IPC_RMID,0);
	msgctl(queue2,IPC_RMID,0);

}

void SendSincr (Messaggio *m, int queue){

	Messaggio m1,m2;
	
	//Costruisco il messaggio READY TO SEND. Lo spedisco al receiver e cerco l'ack
	m1.tipo=REQUEST_TO_SEND;
	strcpy(m1.mess,"Richiesta di invio");
	
	//Invio il messaggio ricorda 3 parametro è il flag
	msgsnd(queue1,&m1,sizeof(Messaggio)-sizeof(long),0);    
	
	//Ricevo l'ok to send dalla coda
	msgrcv(queue2,&m2,sizeof(Messaggio)-sizeof(long),OK_TO_SEND,0);
	
	//Una volta fatto posso mandare il mio messaggio
	msgsnd(queue,m,sizeof(Messaggio)-sizeof(long),0);


}

void ReceiveBloc (Messaggio *m, int queue, int tipomess){

	Messaggio m1,m2;
	
	//Devo ricevere il messaggio RTS
	msgrcv(queue1, &m1, sizeof(Messaggio)-sizeof(long),REQUEST_TO_SEND,0);
	
	//Ora creo e invio il messaggio OKTOSEND
	m2.tipo= OK_TO_SEND;
	strcpy(m2.mess,"Ready to send");
	msgsnd(queue2,&m2,sizeof(Messaggio)-sizeof(long),0);
	
	//Adesso sono pronto per ricevere il messaggio sulla coda
	msgrcv(queue,m,sizeof(Messaggio)-sizeof(long),tipomess,0);

}

void Produttore(int queue, char * text){

	Messaggio m;
	
	//Costruisco il messaggio da trasmettere
	
	m.tipo=MESSAGGIO; //Tipo del messaggio
	strcpy(m.mess,text); //Messaggio effettivo
	
	//Invio il messaggio
	SendSincr(&m, queue);
	printf("MESSAGGIO INVIATO: <%s>\n",m.mess);

}

void Consumatore(int queue){

	Messaggio m;
	
	//Ricevo il messaggio
	ReceiveBloc(&m, queue, MESSAGGIO);
	printf("MESSAGGIO RICEVUTO: <%s>\n",m.mess);

}


