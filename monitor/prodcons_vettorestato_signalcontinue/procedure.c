#include "procedure.h"

#include <unistd.h>
#include <stdio.h>

void Produci(struct ProdCons* pc, int valore){

    //Come prima cosa devo entrare nel monitor
    enter_monitor(&(pc->m));

    printf("Ingresso monitor - produzione\n");

        //Sono dentro al monitor, controllo la variabile condition

        while (pc -> numero_liberi == 0){

            //Se non ci sono liberi mi sospendo
            printf("Sospensione - produzione\n");
            wait_condition( &(pc->m), VARCOND_PRODUTTORI );

            //Mi riattivo dopo che sono stato segnalato
            printf("Riattivazione - produzione\n");

        }

        //Una volta fatto ciò CONTROLLO DOVE POSSO PRODURRE (RICORDA VETTORE DI STATO)

        int i = 0;
        while (i<DIM && pc->stato[i]!= LIBERO){
            i++;
        }

        //A questo punto so dove posso produrre, quindi posso settare in uso, decrementare lib ed usci
        pc->stato[i] = IN_USO;
        pc->numero_liberi--;    //Come vedi per ora mi interesso solol dei liberi x vettore stato

    //RICORDA ! CON VETTORE DI STATO DEVO USCIRE DAL MONITOR PRIMA DI PRODURRE !
    //IN QUESTO MODO PERMETTO AD ALTRI PRODUTTORI DI ENTRARE NEL FRATTEMPO E CHECKARE
    leave_monitor(&(pc->m));

    sleep(2);

    //Produco
    pc->buffer[i] = valore;
    printf("Produzione - posizione %d, valore %d\n", i, valore);

    //Rientro nel monitor perchè devo settare a OCCUPATO ed aumentar eil numero occupati
    enter_monitor(&(pc->m));

        //Sono nel monitor
        pc->stato[i] = OCCUPATO;
        pc->numero_occupati++;

    //A QUESTO PUNTO POSSO SEGNALARE AI CONSUMATORI CHE STANNO ASPETTANDO SU QUELLA CONDIZIONE
    signal_condition(&(pc->m),VARCOND_CONSUMATORI);

    //ESCO DAL MONITOR
    leave_monitor( &(pc->m) );

	printf("Uscita monitor - produzione\n");

}

int Consuma(struct ProdCons* pc){

    int valore;

    //Come prima cosa devo entrare nel monitor
    enter_monitor(&(pc->m));

    printf("Ingresso monitor - consumazione\n");

        //SONO DENTRO AL MONITOR. DEVO FARE IL CHECK SULLA CONDIZIONE
        while (pc->numero_occupati==0){

            //Se non posso consumare niente aspetto la signal
            printf("Sospensione - consumazione\n");
            wait_condition( &(pc->m), VARCOND_CONSUMATORI );

            //Ripresa
            printf("Riattivazione - consumazione\n");

        }

        //SONO SEMPRE DENTRO AL MONITOR. CHECKO DOVE POSSO CONSUMARE
        int i = 0;

        while (i<DIM && pc->stato[i]!=OCCUPATO){
            i++;
        }

        //A QUESTO PUNTO HO CAPITO DOVE POSSO CONSUMARE. QUINDI AGGIORNO A IN_USO E DECREMENTO OCCU
        pc->stato[i] = IN_USO;
        pc->numero_occupati--;

    //RICORDA ! CON VETTORE DI STATO DEVO USCIRE DAL MONITOR PRIMA DI CONSUMARE !
    //IN QUESTO MODO PERMETTO AD ALTRI CONSUMATORI DI ENTRARE NEL FRATTEMPO E CHECKARE
    leave_monitor(&(pc->m));

    //Ora posso consumare
    valore = pc->buffer[i];

    //Adesso devo rientrare nel monitor per fare la signal ai produttori che possono produrre
    enter_monitor(&(pc->m));

        //Sono dentro al monitor. Setto a libero, aumento numero liberi e segnalo
        pc->stato[i] = LIBERO;
        pc->numero_liberi++;
        signal_condition(&(pc->m),VARCOND_PRODUTTORI);

    //Esco dal monitor
    leave_monitor(&(pc->m));

    printf("Uscita monitor - consumazione\n");


    return valore;

}
