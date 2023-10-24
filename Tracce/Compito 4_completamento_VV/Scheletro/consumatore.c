#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>
#include "header.h"


int main() {
	int pid;
	//TODO: completare l'inizializzazione code
    key_t msg_req_key = ftok(FTOK_PATH,FTOK_CHAR_RICHIESTE);
	key_t msg_res_key = ftok(FTOK_PATH,FTOK_CHAR_RISPOSTE);

	int queue_req = msgget(msg_req_key,IPC_CREAT|0664);
	int queue_res = msgget(msg_res_key,IPC_CREAT|0664);
	
	if (queue_req < 0 || queue_res < 0){
		exit(1);
	}
	
	int i;
	
	for (i=0; i < NUM_CONS; i++) {
	
		req msg;
		//TODO preparazione messaggio
		
		msg.type = TIPOMESS;
		
		//TODO invio richiesta
		
		msgsnd(queue_req,&msg,sizeof(msg)-sizeof(long),0);
		printf("CONSUMATORE %d: inviata richiesta. Attendo una risposta \n",getpid());
		
		
		res risp;
		//TODO ricezione risposta
		msgrcv(queue_res,&risp,sizeof(msg)-sizeof(long),0,0);
		printf("CONSUMATORE %d: letto valore: %d\n",getpid(),risp.value);
		sleep(1);
	}
	
	//TODO rimozione code
	msgctl(queue_req,IPC_RMID,0);
	msgctl(queue_res,IPC_RMID,0);
	printf("[CONSUMATORE %d: rimosse correttamente le code\n",getpid());

	return 0;
}
