#include "header.h"
#include "monitor_signal_continue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/wait.h>

int main(){
	
	//Variabili utili
	pid_t pid;
	int i;
	int numero_processi = NUMERO_PRODUTTORI + NUMERO_CONSUMATORI + NUMERO_FILTRI + NUMERO_CHECKSUM + NUMERO_VISUALIZZATORI;
	
	//Shmem
		key_t key_shmem = IPC_PRIVATE;
		
		//Genero la shared memory
		int id_shmem = shmget(key_shmem,sizeof(ProdCons),IPC_CREAT|0664);
		
		//Check
		if (id_shmem < 0){
			perror("Errore shmget\n");
			exit(1);
		}
		
		//Mi attacco alla shared memory
		ProdCons * p = (ProdCons*) shmat(id_shmem,0,0);
		
		//Check
		if (p == (void*) -1){
			perror("Errore shmat\n");
			exit(1);
		}
		
		//Inizializzo la struttura prodcons
		inizializza_prodcons(p);
	
	//Code
		key_t key_gen_filter = IPC_PRIVATE;
		key_t key_filter_checksum = IPC_PRIVATE;
		key_t key_checksum_visual = IPC_PRIVATE;
		
		int id_gen_filter = msgget(key_gen_filter, IPC_CREAT | 0644);
        int id_filter_checksum = msgget(key_filter_checksum, IPC_CREAT | 0644);
        int id_checksum_visual = msgget(key_checksum_visual, IPC_CREAT | 0644);
	
		printf(" -- Sistema -- Code create\n");
	
	
	//Genero i processi figli
	
		//Processo generatore
		for (i=0;i<NUMERO_PRODUTTORI+NUMERO_CONSUMATORI;i++){
		
			pid = fork();
				
			if (pid == 0){
				
				srand(time(NULL)*getpid()*i);
				//Produttori
				if (i%2 == 0){
				
					Produttore(p);					
				
				}
				
				//Consumatori
				else{
				
					Consumatore(p,id_gen_filter);
				
				}
				
				exit(0);
			
			}
			
		
		}
		
		//Processo filtro
		pid = fork();
		
		if (pid == 0){
			
			Filtro(id_gen_filter,id_filter_checksum);
			
		}
		
		//Processo checksum
		pid = fork();
		
		if (pid == 0){
			
			Checksum(id_filter_checksum,id_checksum_visual);
			
		}
		//Processo visual
		pid = fork();
		
		if (pid == 0){
			
			Visualizzatore(id_checksum_visual);
			
		}
	
	
	//Wait sulla terminazione
	for (i=0;i<numero_processi;i++){
		wait(NULL);
		printf(" [[[[[[PROCESSO %d ]]]]]] terminato.\n",i+1);
	}
	
	//Cleanup risorse
	rimuovi_prodcons(p,id_shmem);
	remove_msgqueue(id_gen_filter);
	remove_msgqueue(id_filter_checksum);
	remove_msgqueue(id_checksum_visual);
	printf(" -- Sistema -- Code rimosse\n");

	return 0;
	
}
