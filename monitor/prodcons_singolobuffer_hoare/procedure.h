#ifndef _HEADER_H
#define _HEADER_H

#include "monitor_hoare.h"

//Dichiaro la struct che deve contenere il monitor, le variabili condition e cosa voglio proteggere

struct ProdCons{

    //Shmem
    int buffer;

    //Condvar
    int buffer_libero;  //1 Se libero, 0 se pieno
    int buffer_occupato; //1 Se occupato, 0 se libero

    //Monitor
    Monitor m;

};

#define VARCOND_PRODUTTORI 0
#define VARCOND_CONSUMATORI 1

void Produci(struct ProdCons* pc, int valore);
int Consuma(struct ProdCons* pc);

#endif
