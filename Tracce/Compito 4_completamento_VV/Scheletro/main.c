#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>
#include "header.h"
int main(){
    int pid;
    
	//TODO: completare l'inizializzazione code
    key_t msg_req_key = ftok(FTOK_PATH,FTOK_CHAR_RICHIESTE);
	key_t msg_res_key = ftok(FTOK_PATH,FTOK_CHAR_RISPOSTE);

    int queue_req = msgget(msg_req_key,IPC_CREAT|0664);
	int queue_res = msgget(msg_res_key,IPC_CREAT|0664);

    pid = fork();
    if (pid==0) {
        //TODO: exec del server
        execv("./server",NULL);
        _exit(0);
    }

    pid = fork();
    if (pid==0) {
        //TODO: exec del consumatore
        execv("./consumatore",NULL);
        _exit(0);
    }

    wait(NULL);
    wait(NULL);

    //TODO: eliminare le code
   	msgctl(queue_req,IPC_RMID,0);
	msgctl(queue_res,IPC_RMID,0);

    return 0;
}
