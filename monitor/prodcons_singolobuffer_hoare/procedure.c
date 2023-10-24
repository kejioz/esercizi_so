#include "procedure.h"

#include <stdio.h>

void Produci(struct ProdCons* pc, int valore){

    //Per prima cosa devo entrare nel monitor. Gli passo il riferimento al monitor contenuto in pc
    enter_monitor(&(pc->m));

        //Sono dentro al monitor
        printf("Ingresso al monitor - Produzione\n");

        //Controllo la condizione con la variabile condition di prodcons
        if ( pc->buffer_occupato == 1){

            //Se buffer_occupato è 1, ovvero il buffer è occupato, faccio la wait sulla condvar
            printf("[PRODUTTORE] Il buffer è occupato, mi sospendo sulla condvar\n");
            wait_condition(&(pc->m),VARCOND_PRODUTTORI);

            //Quando passo qua vuol dire che qualcuno ha consumato e sign varcond VARCOND_PRODUTTORI
            printf("[PRODUTTORE] Signal ricevuta, posso riprendere la produzione\n");

        }

        //Quando sono qui posso produrre e settare di nuovo le condition variables
        pc->buffer = valore;
        pc->buffer_libero = 0;
        pc->buffer_occupato = 1;
        printf("[PRODUTTORE] Prodotto con successo %d\n",valore);

        //A questo punto posso fare la signal sulla condition variable dei consumatori
        signal_condition(&(pc->m),VARCOND_CONSUMATORI);

    //UNA VOLTA FINITO TUTTO POSSO USCIRE DAL MONITOR
    leave_monitor(&(pc->m));

    printf("Uscita dal monitor - Produzione\n");

}


int Consuma(struct ProdCons* pc){

    int valore;
    //Per prima cosa devo entrare nel monitor.
    enter_monitor(&(pc->m));

        //SONO DENTRO AL MONITOR
        printf("Ingresso al monitor - Consumazione\n");

        //Controllo il valore della condition variable. Se è vuoto allora devo fare la wait sulla cond
        if (pc->buffer_libero == 1){

            //Se buffer_libero è 1 vuol dire che nn ci sta niente e devo aspettare la produzione
            printf("[CONSUMATORE] Il buffer è vuoto, mi sospendo sulla condvar\n");
            wait_condition(&(pc->m),VARCOND_CONSUMATORI);

            //Quando passo qua vuol dire che qualcuno ha segnalato varcond consumatori e posso ripr
             printf("[CONSUMATORE] Signal ricevuta, posso riprendere la consumazione\n");
        }

        //A questo punto posso consumare e aggiornare le variabili condition
        valore = pc->buffer;
        pc->buffer_libero=1;
        pc->buffer_occupato=0;

        //Segnalo ai produttori che possono produrre
        signal_condition(&(pc->m),VARCOND_PRODUTTORI);

    //UNA VOLTA FINITO TUTTO POSSO USCIRE DAL MONITOR E RITORNARE IL VALORE
    leave_monitor(&(pc->m));
    printf("Uscita dal monitor - Consumazione\n");

    return valore;
}
