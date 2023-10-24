#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

	//Faccio una system call per creare un nuovo processo
	//pid_t è un signed int che rappresenta i process id
	pid_t pid=fork();

	//Check chiamata fork non fallita
	if (pid ==-1){

		printf("Fork fallita\n");

	}

	else if (pid ==0){

		pid_t mypid = getpid();
		printf("Ciao, sono il processo figlio di %d con pid %d spawnato con la fork\n",getppid(),mypid);
		exit(0);

	}

	else if (pid>0){

		printf("Ciao, sono il processo padre. Il figlio è %d, aspetto la sua terminazione.\n",pid);
		int childpid = wait(NULL);
		printf("Sono sempre il padre, mio figlio %d ha terminato. Adesso spawno altri figli\n",childpid);
		
		int pidfigli;
		for (int i=0;i<5;i++){
		
			//Forko
			pidfigli = fork();
			
			//Se sono il figlio non devo forkare
			if (pidfigli == 0){
			
				printf ( "Ciao, sono il processo figlio di %d con pid %d\n",getppid(),getpid());
				i=5;
				exit(0);
			
			}
		
		}
		
		printf("Sono il padre, attendo per la terminazione dei miei figli\n");
		
		for (int i=0;i<5;i++){
		
			int pidterminato = wait(NULL);
			
			printf("Finarmente er figlio %d ha terminato! \n",pidterminato);
		
		}

	}

	return 0;
}
