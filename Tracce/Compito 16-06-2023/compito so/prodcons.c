#include <stdio.h>

#include "prodcons.h"


void init_monitor_buffer_singolo(MonitorBufferSingolo * b) {

    /* TBD: Inizializzare il monitor */
    
    //Inizializzo il monitor con 2 cv
    init_monitor(&b->m,2);
    
    b->stato_buffer = VUOTO;
    
}

void produzione_buffer_singolo(MonitorBufferSingolo * b, int val) {

    /* TBD: Completare la produzione sul buffer singolo con sincronizzazione */
	
	//Entro nel monitor
	enter_monitor(&b->m);
	
		//Check cv, hoare quindi if va bene
		if (b->stato_buffer == PIENO){
			
			//Se il buffer è pieno vado in wait
			wait_condition(&b->m,CV_PRODUTTORE);
			
		}
		
		//Produco e setto il buffer di nuovo a pieno
    	b->buffer = val;
    	b->stato_buffer = PIENO;
    	
    	//Segnalo i consumatori ed esco dal monitor
    	signal_condition(&b->m,CV_CONSUMATORE);
    	
    leave_monitor(&b->m);

}

int consumazione_buffer_singolo(MonitorBufferSingolo * b) {

    /* TBD: Completare la consumazione dal buffer singolo con sincronizzazione */

    int val;
    
    //Entro nel monitor
	enter_monitor(&b->m);
	
		//Check cv, hoare quindi if va bene
		if (b->stato_buffer == VUOTO){
			
			//Se il buffer è vuoto vado in wait
			wait_condition(&b->m,CV_CONSUMATORE);
			
		}

    	val = b->buffer;
    	b->stato_buffer = VUOTO;
    	
    	//Segnalo i produttori ed esco dal monitor
    	signal_condition(&b->m,CV_PRODUTTORE);
    	
    leave_monitor(&b->m);

    return val;
}

void remove_monitor_buffer_singolo(MonitorBufferSingolo * b) {

    /* TBD: Disattivare l'oggetto monitor */
    remove_monitor(&b->m);
    
}
