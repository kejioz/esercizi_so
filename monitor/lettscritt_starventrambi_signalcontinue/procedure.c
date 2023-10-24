#include "procedure.h"

#include <unistd.h>
#include <stdio.h>

int Leggi(struct LettScritt * ls) {

    int valore;

    //Per prima cosa entro nel monitor
    enter_monitor(&(ls->m));

    printf("Lettura - ingresso monitor\n");

        //Un lettore può leggere se non ci sono scrittori, quindi check sulla variabile condition
        if ( ls->numero_scrittori > 0){

            printf("[LETTORE] Mi sospendo, è occupato da scrittore");
            wait_condition(&ls->m,VARCOND_LETTORI);

            //Questo è un signal and continue. Saranno gli scrittori a segnalare
            //NON SEGNALO QUINDI GLI ALTRI LETTORI
            printf("[LETTORE] Mi riattivo");
        }

        //Sono dentro, quindi devo aumentare il numero di lett
        ls->numero_lettori++;

        printf("Numero lettori ++ : %d\n", ls->numero_lettori);

        //Questo è un signal and continue. Saranno gli scrittori a segnalare
        //NON SEGNALO QUINDI GLI ALTRI LETTORI

    //Posso uscire dal monitor
    leave_monitor(&(ls->m));

    //A questo punto posso leggere
    //NOTA CHE POSSO LEGGERE PERCHÈ TUTTI GLI SCRITTORI POSSONO LEGGERE INSIEME
    //E SONO SICURO CHE NON HO SCRITTORI CHE ROMPONO PERCHÈ NUMERO_LETTORI È ALMENO 1
    sleep(2);

    valore = ls->buffer;

    printf("Lettura - valore [%d]\n", valore);

    //Una volta che ho finito devo decrementare il numero di lettori, quindi rientrare
    enter_monitor(&(ls->m));

        ls->numero_lettori--;

        //L'ultimo lettore ad abbandonare la risorsa deve segnalare gli scrittori
        if ( ls->numero_lettori == 0){

            printf("Lettura - signal su scrittori\n");
            signal_condition( &(ls->m), VARCOND_SCRITTORI );
        }

    //Posso uscire dal monitor
    leave_monitor( &(ls->m) );

	printf("Lettura - uscita monitor\n");

	return valore;


}


void Scrivi(struct LettScritt * ls, int valore) {

    //Come prima cosa entro nel monitor
	enter_monitor( &(ls->m) );
	printf("Scrittura - ingresso monitor\n");

        //Una volta che sono dentro faccio il check sulla condition variable
        //Nota che la condizione è che devo essere l'unico scrittore e non ci devnon essere lettori'
        if (ls->numero_scrittori > 0 || ls->numero_lettori >0){

            printf("Scrittura - sospensione\n");
            wait_condition( &(ls->m), VARCOND_SCRITTORI );

            //Quello che mi segnalerà sarà l'ultimo lettore, prima di andarsene
            printf("Scrittura - riattivazione\n");

        }

        //Aumento il numero scrittori in modo da poter scrivere in santa pace
        ls->numero_scrittori++;
        printf("Numero scrittori ++ : %d\n", ls->numero_scrittori);

    //A questo punto posso uscire dal monitor per scrivere
    leave_monitor( &(ls->m) );

    sleep(1);

    //Scrivo
    ls->buffer = valore;

	printf("Scrittura - valore [%d]\n", valore);

    //Devo rientrare nel monitor per dire che ho finito e decrementare la variabile
    enter_monitor( &(ls->m) );

        ls->numero_scrittori--;

        printf("Numero scrittori -- : %d\n", ls->numero_scrittori);

        //Adesso verifico se ci sono scrittori in attesa: se ci sono è il loro turno, altrimenti lett
        if (queue_condition( &(ls->m),VARCOND_SCRITTORI)){

            printf("Scrittura - signal su scrittori\n");

            signal_condition( &(ls->m), VARCOND_SCRITTORI );
        }

        else{

        /*
		 *  NOTA: questa soluzione è applicabile solo al caso di
		 *  monitor con semantica signal-and-continue (signal all).
         *  Questo perchè lo scrittore dopo aver fatto la prima signal
         *  CONTINUA la sua esecuzione!
        */

            printf("Scrittura - signal all su lettori\n");

            signal_all(&(ls->m),VARCOND_LETTORI);

        }

    //Finito, esco dal monitor
    leave_monitor( &(ls->m) );

	printf("Scrittura - uscita monitor\n");
}


