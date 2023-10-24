#include "semafori.h"
#include "procedure.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <limits.h>

void IniziaLettura(int ds_sem, Buffer* buf){

    //Come prima cosa provo a vedere se qualcuno sta già provando a modificare la variabile numerolettori
    Wait_Sem(ds_sem,MUTEXL);

        buf->numlettori = buf ->numlettori+1;
        printf("Numero corrente di lettori: %d \n",buf->numlettori);
        //Se sono il primo lettore, allora vuol dire che devo mettere in wait gli scrittori
        if (buf->numlettori == 1){

            Wait_Sem(ds_sem,SYNCH);

        }

    //Una volta finito posso segnalare agli altri lettori che ho con successo aumentato il numero di lettori e possono cominciare anche loro la lettura
    Signal_Sem(ds_sem,MUTEXL);


}

void FineLettura(int ds_sem, Buffer* buf){

    //Come prima cosa provo a vedere se qualcuno sta già provando a modificare la variabile numerolettori
    Wait_Sem(ds_sem,MUTEXL);

        buf->numlettori = buf ->numlettori-1;
        
        //Se sono l'ultimo lettore vuol dire che posso segnalare gli scrittori che è il loro turno
        if ( buf->numlettori == 0){

            Signal_Sem(ds_sem,SYNCH);

        }

    //Resetto il semaforo
    Signal_Sem(ds_sem,MUTEXL);
    
}

void IniziaScrittura(int ds_sem, Buffer* buf){

    //Mutua esclusione su variabile condivisa numero scrittori
    Wait_Sem(ds_sem,MUTEXS);

        buf->numscrittori = buf->numscrittori+1;

        //Se sono il primo scrittore, allora vuol dire che devo far waitare i lettori
        if (buf -> numscrittori == 1){

            Wait_Sem(ds_sem,SYNCH);

        }

    //Posso rilasciare il mutex per permettere di modificare la variabile agli altri scrittori
    Signal_Sem(ds_sem,MUTEXS);

    //Adesso basta solo gestire la mutua esclusione con gli altri scrittori con MUTEX
    Wait_Sem(ds_sem, MUTEX);



}

void FineScrittura(int ds_sem, Buffer* buf){

    //Adesso ho finito di scrivere e quindi SEGNALO IL SEMAFORO IN MODO DA FAR REINSERIRE NELLA CODA DEI PROCESSI PRONTI GLI SCRITTORI CHE AVEVANO WAITATO
    Signal_Sem(ds_sem,MUTEX);

    //Mutua esclusione su variabile condivisa numero scrittori
    Wait_Sem(ds_sem,MUTEXS);

        buf->numscrittori = buf->numscrittori-1;

        //Se sono l'ultimo scrittore, allora vuol dire che devo far riprendere i lettori
        if (buf -> numscrittori == 0){

           Signal_Sem(ds_sem,SYNCH);

        }
    
    //Posso rilasciare il mutex per permettere di modificare la variabile agli altri scrittori che vogliono inziare la scrittura
    Signal_Sem(ds_sem,MUTEXS);


}

void Lettore(int ds_sem, Buffer* buf){

    //Devo proteggere tutto con un inizia lettura e con un fine lettura
    IniziaLettura(ds_sem,buf);

        printf("[LETTORE] Ho letto con successo il messaggio immagazzinato nel buffer : << %ld >>\n",buf->messaggio);

    FineLettura(ds_sem,buf);

}

void Scrittore(int ds_sem, Buffer* buf){

    //Devo proteggere tutto con un inizia scrittura e con un fine scrittura
    IniziaScrittura(ds_sem,buf);

        buf->messaggio = (long) rand() % LONG_MAX;
        printf("[SCRITTORE] Ho scritto con successo nel buffer : << %ld >>\n",buf->messaggio);

    FineScrittura(ds_sem,buf);


}
