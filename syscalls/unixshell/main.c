#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

int main(int argc, char * argv[]) {
		
		
		int pid = fork();
		
		//Se la fork fallisce
        if( pid == -1 ) {
                printf("Fork fallita.");
                exit(1);
        }

		//Se la fork va a buon fine e sono il figlio.
        else if( pid == 0 ) {

			//Attendo 2 secondi
            for(int i=0; i<2; i++) {
                sleep(1);
                printf("...\n");
            }

            //OPERAZIONI PER PRENDERE LE FLAGS

            //Per prima cosa alloco un array di puntatori a char, quindi un puntatore a puntatori di char di dimensione argc ( numero di stringhe)
            char** flags = (char**) malloc(sizeof(char*)*argc);

            //Scorro tutti quanti i puntatori di puntatori a char
            for (int i=0;i<argc-2;i++){

                //Inizializzo ogni puntatore allocando n bytes quanto la lunghezza del comando
                *(flags+i) = (char*) malloc(sizeof(char)*strlen(argv[i+2]));
                //Copio il comando nell'array'
                strcpy(*(flags+i),argv[i+2]);
                printf("Parametro %d : %s\n",i+1,*(flags+i));

            }

            //In questo caso uso execvp, siccome voglio passare i parametri tramite array e voglio passare solo il nome dell'eseguibile, siccome non conosco il path'
            execvp(argv[1],flags);
            
            //Freeing memory
            for (int i=0;i<argc-2;i++){

                free(*(flags+i));

            }

            free(flags);

            //NON DEVO ENTRARE IN QUESTA PARTE DEL CODICE ! SE CI ENTRO DEVO KILLARE, VUOL DIRE KE CIE PROBLEMA
       		printf("C'È STATO UN PROBLÒEMA KILLA TUTTOOOO...\n");
       		kill(getppid(),SIGKILL);
       		exit(1);
            
        }
        
        //Se sono il padre
        else if ( pid > 0){
        
        	    printf("Sono il processo padre, con PID %d\n", getpid());

                wait(NULL);

                printf("Call di %s effettuata con successo!\n",argv[1]);
                printf("Il processo padre termina\n");

                exit(0);
        
        
        }
        
        return 0;
        
}
        
