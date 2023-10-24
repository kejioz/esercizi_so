#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <string.h>

int main(){
	
	//Per prima cosa genero una key ipc utilizzando 'KEY_T'
	//In ftok il primo parametro è il pathname e il secondo una stringa a caso. Usando gli stessi ho stesse key
	key_t shm_key = ftok(".",'k');
	
	//Una volta che ho la key, la utilizzo per ottenere il descrittore ipc con una get
	//Primo parametro la key, secondo la size della memoria, e poi i flag se CREAT o EXCL ed i permessi
	int descrittore_shm = shmget(shm_key, 100, IPC_CREAT | IPC_EXCL | 0664);
	//In questo caso do la key, 100 bytes, e facendo or in questo modo mi ritorna un errore se la risorsa esiste già
	
	//Puntatore
	char * p;
	
	//Se viene tornato -1 vuol dire che la risorsa già esiste. La prendo con una get normale
	if (descrittore_shm<0){
	
		//La risorsa già esiste, la getto semplicemente
		descrittore_shm = shmget (shm_key,100,0664);
		
		//Mi attacco al descrittore tramite la syscall shmat
		//Il primo parametro è l'identificatore del segmento
		//Il secondo parametro è l'indirizzo di memoria del processo chiamante al quale voglio fare l'attach
			//Se utilizzo 0, il kernel trova automaticamente una regione di memoria
		//Il terzo parametro indica dei flag, ad esempio IPC_RDONLY sola lettura
		p = (char*) shmat(descrittore_shm, NULL, 0);
		
		/*La funzione shmat() restituirà l'indirizzo di memoria, nello spazio di indirizzamento del processo chiamante,
		al quale il segmento di memoria condivisa è stato collegato (attached). In caso di fallimento, la funzione
		restituirà il valore -1.*/

	}
	
	//Se invece la risorsa è appena stata creata 
	else{
	
		//Mi attacco al descrittore tramite la syscall shmat
		p = (char*) shmat(descrittore_shm, NULL, 0);
		strncpy (p, "HELLO WORLD", sizeof ("HELLO WORLD"));
	
	}

	printf("Dettagli shared memory creata:\n");
    printf("...chiave IPC shm: %d\n", shm_key);
	printf("...descrittore shm: %d\n", descrittore_shm);
	printf("...indirizzo dopo attach: %p\n", p);

 	printf("Contenuto SHM: %s\n", p);

	//La funzione shmctl permette di evocare un comando di controllo su una shm esistente e restituisce -1 se fallisce
	//Il primo parametro è il descrittore
	//Il secondo parametro è il COMANDO DA ESEGUIRE
		//STAT : copia le informazioni di stato da shmid a buf
		//SET: fa scrivere i valori della shmid in byuf
		//RMID: dice di eliminare la shmem quando non ci sono processi attached
		//LOCK: impedisce che il segmento venga swappato o paginato
	//Il terzo parametro , buf, punta ad un shmid_ds ed è input se uso IPC_SET ed output se uso IPC_STAT
    shmctl(descrittore_shm, IPC_RMID, NULL);

    return 0;
}
