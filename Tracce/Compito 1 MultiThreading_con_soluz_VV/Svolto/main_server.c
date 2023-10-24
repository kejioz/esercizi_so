#include <sys/msg.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "header.h"

typedef struct {

	int valore;
	thread_buffer* tb;

} parametri_thread;

void update_value(thread_buffer* tb, int value) {

	pthread_mutex_lock(&tb->mutex);
	
	int tid = syscall(SYS_gettid);
	
	printf("[%d] Thread: buffer %d.\n", tid, value);
	
	tb->buffer += value;
	
	pthread_mutex_unlock(&tb->mutex);

}

void * Worker(void * par_t) {

	parametri_thread* my_par_t = (parametri_thread*) par_t;
	
	update_value(my_par_t->tb, my_par_t->valore);
	
	pthread_exit(0);

}

int main(int agrc, char*argv) {

	pthread_attr_t attr;
	pthread_t threads[NUM_THREADS];
	
	key_t msg_key = ftok(".",'.');
	int msgid = msgget(msg_key,0);
	
	if (msgid<0) {perror("errore coda server"); exit(1);}
	
	thread_buffer * tb = malloc(sizeof(thread_buffer));
	tb->buffer=0;
	pthread_mutex_init(&tb->mutex, NULL);
	
	parametri_thread pt[NUM_THREADS];
	
	printf("[Server] Valore buffer iniziale: %d.\n", tb->buffer);
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	int t;
	
	for (t=0; t<NUM_THREADS; t++) {
	
		messaggio mess;
	
		msgrcv(msgid, &mess, sizeof(messaggio)-sizeof(long), 0, 0);
		
		pt[t].valore = mess.msg;
		pt[t].tb = tb;
		
		printf("[%d] Server: creo thread %d con messaggio %d.\n", getpid(), t, pt[t].valore);
		
		pthread_create(&threads[t], &attr, Worker, (void*)&pt[t]);
	
	}
	
	for (int t=0; t<NUM_THREADS; t++) {
		pthread_join(threads[t], NULL);
		printf("[%d] Thread terminato.\n", t);
	}
	
	printf("[Server] Valore buffer finale: %d.\n", tb->buffer);

	free(tb);
	pthread_mutex_destroy(&tb->mutex);
	pthread_attr_destroy(&attr);
	msgctl(msgid, IPC_RMID, 0);

	return 0;
	
}

