#include <sys/msg.h>
#include <sys/types.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "header.h"

int main(int agrc, char*argv) {

	key_t msg_key = ftok(".",'.');
	int msgid = msgget(msg_key, 0);
	
	if (msgid<0) {perror("errore coda client"); exit(1);}
	
	for (int i=0; i<MSG_CLIENT; i++) {
	
		messaggio mess;
		mess.type = TYPE;
		mess.msg = i;
		
		printf("[%d] Client: invio messaggio: %d.\n", getpid(), i);
	
		msgsnd(msgid, &mess, sizeof(messaggio)-sizeof(long), 0);
	
	}

	return 0;
	
}
