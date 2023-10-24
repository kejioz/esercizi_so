#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char * argv[]) {
		
		
		int pid = fork();
		
		//Se la fork fallisce
        if( pid == -1 ) {
                printf("Fork fallita.");
                exit(1);
        }

		//Se la fork va a buon fine e sono il figlio.
        else if( pid == 0 ) {

			//Prompto le informazioni. Argv0 sarà il comando ed argv1,2 saranno i flags	
            printf("Sono il processo figlio di %d , il mio PID è PID: %d\n",getppid() ,getpid());
            printf("Command line passata al main():\n");
            printf("argv[0]: %s\n", argv[0]);
            printf("argv[1]: %s\n", argv[1]);
            printf("argv[2]: %s\n", argv[2]);
            printf("Attendo 4 secondi..\n");

			//Attendo 4 secondi
            for(int i=0; i<3; i++) {
                sleep(1);
                printf("...\n");
            }
            
            //Adesso posso fare exec. Uso execl con la lista ed il NULL, altrimenti con execv avrei usato vettore. IL PRIMO PARAMETRO È IL PATH DEL COMANDO ( COMANDO INCLUSO) E IL SECONDO IL COMANDO PROPRIO
            execl("/bin/cp","cp",argv[1],argv[2],NULL);
            
            //NON DEVO ENTRARE IN QUESTA PARTE DEL CODICE ! SE CI ENTRO DEVO KILLARE, VUOL DIRE KE CIE PROBLEMA
       		printf("C'È STATO UN PROBLÒEMA KILLA TUTTOOOO...\n");
       		kill(getppid(),SIGKILL);
       		exit(1);
            
        }
        
        //Se sono il padre
        else if ( pid > 0){
        
        	    printf("Sono il processo padre, con PID %d\n", getpid());

                wait(NULL);

                printf("Copia effettuata con successo!\n");
                printf("Il processo padre termina\n");

                exit(0);
        
        
        }
        
        return 0;
        
}
        
