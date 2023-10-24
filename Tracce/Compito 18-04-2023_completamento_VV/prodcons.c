#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "prodcons.h"

void inizializza(MonitorPC * m) {

	int i;
    printf("Inizializzazione monitor\n");

    /* TBD: Inizializzazione */
    
    //Prima inizializzo tutte le caselle del buffer a libero
    for (i=0;i<DIM;i++){
    	m->stato[i] = LIBERO;
    }
    
    m->num_liberi = DIM;
    m->num_occupati_tipo1 = 0;
    m->num_occupati_tipo2 = 0;
    
    //Poi inizializzo il monitor con init monitor
    init_monitor(&(m->m),4);
    
    printf(" /// SISTEMA /// Inizializzato il monitor con 4 condvar, e il vettore di stato a libero\n");

}


void rimuovi(MonitorPC * m) {

    printf("Rimozione monitor\n");

    /* TBD: Rimozione */
    remove_monitor(&(m->m));
    
    printf(" /// SISTEMA /// Rimosso il monitor. \n");

}


void produci_tipo_1(MonitorPC * m, int valore) {

    int index = 0;

    /* TBD: Aggiungere la sincronizzazione, 
            con la selezione dell'indice su cui produrre
            (alg. con vettore di stato) */
            
    //Per prima cosa entro nel monitor
    enter_monitor(&m->m);
    
    
    	//Check sulla variabile condition uso while siccome sig continue
    	while (m->num_liberi == 0){
    		printf("[PROD1] Non ci sono liberi, vado in wait.\n");
    		wait_condition(&m->m,CV_PROD1);
    		printf("[PROD1] Risvegliato!\n");
    	}
    	
    	//Sono dentro. Scorro il vettore di stato fin quando non trovo libero
    	while (index < DIM && m->stato[index] != LIBERO){
    	
    		//Fintantochè l'indice è nella dim, e non trovo un libero vado avanti
    		index++;
    	
    	}
    	
    	printf("[PROD1] Trovata casella nella quale posso produrre\n");
    	
    	//Setto la casella ad in uso
    	m->stato[index] = INUSO;
    	
    	//Decremento il numero di liberi
    	m->num_liberi--;
    	
    //A questo punto ho capito dove posso produrre. Esco dal monitor
    leave_monitor(&m->m);
    
    ///Produco 	
    printf("Produzione di tipo 1 in corso (valore=%d, index=%d)\n", valore, index);

    sleep(1);
    
    m->vettore[index] = valore;
    
    //Rientro nel monitor per settare a occupato1
    enter_monitor(&m->m);
    
    	//Modifico la variabile di stato a occupato1, aumento il n di occupati
    	m->stato[index] = OCCUPATO1;
    	m->num_occupati_tipo1++;
    	
    	//Segnalo sui consumatori 1 che possono consumare
    	signal_condition(&m->m,CV_CONS1);
    	
    //Esco dal monitor
    leave_monitor(&m->m);

}


void produci_tipo_2(MonitorPC * m, int valore) {

    int index = 0;

    /* TBD: Aggiungere la sincronizzazione, 
            con la selezione dell'indice su cui produrre
            (alg. con vettore di stato) */
            
    //Per prima cosa entro nel monitor
    enter_monitor(&m->m);
    
    
    	//Check sulla variabile condition uso while siccome sig continue
    	while (m->num_liberi == 0){
    		printf("[PROD2] Non ci sono liberi, vado in wait.\n");
    		wait_condition(&m->m,CV_PROD2);
    		printf("[PROD2] Risvegliato!\n");
    	}
    	
    	//Sono dentro. Scorro il vettore di stato fin quando non trovo libero
    	while (index < DIM && m->stato[index] != LIBERO){
    	
    		//Fintantochè l'indice è nella dim, e non trovo un libero vado avanti
    		index++;
    	
    	}
    	
    	printf("[PROD2] Trovata casella nella quale posso produrre\n");
    	
    	//Setto la casella ad in uso
    	m->stato[index] = INUSO;
    	
    	//Decremento il numero di liberi
    	m->num_liberi--;
    	
    //A questo punto ho capito dove posso produrre. Esco dal monitor
    leave_monitor(&m->m);
    
    ///Produco 	
    printf("Produzione di tipo 2 in corso (valore=%d, index=%d)\n", valore, index);

    sleep(1);
    
    m->vettore[index] = valore;
    
    //Rientro nel monitor per settare a occupato2
    enter_monitor(&m->m);
    
    	//Modifico la variabile di stato a occupato2, aumento il n di occupati
    	m->stato[index] = OCCUPATO2;
    	m->num_occupati_tipo2++;
    	
    	//Segnalo sui consumatori 2 che possono consumare
    	signal_condition(&m->m,CV_CONS2);
    	
    //Esco dal monitor
    leave_monitor(&m->m);
}


void consuma_tipo_1(MonitorPC * m, int * valore) {

    int index = 0;

    /* TBD: Aggiungere la sincronizzazione, 
        con la selezione dell'indice su cui consumare
        (alg. con vettore di stato) */
	
	//Per prima cosa entro nel monitor
	enter_monitor(&m->m);
	
		//Sono dentro. Check sulla condvar
		while (m->num_occupati_tipo1 == 0){
			printf("[CONS1] No tipo1, vado in wait\n");
			wait_condition(&m->m,CV_CONS1);
			printf("[CONS1] Risvegliato\n");
		}
		
		//A questo punto cerco un occupato1
		while (index < DIM && m->stato[index]!= OCCUPATO1){
		
			//Fintantokè sto nel vettore e non trovo una casella occupata1 vado avanti
			index++;
			
		}
		
		printf("[CONS1] Trovata casella nella quale posso consumare\n");
		
		//A questo punto setto la casella a inuso e decremento occ di tipo 1
		m->stato[index] = INUSO;
		m->num_occupati_tipo1--;
		
	//Lascio il monitor
	leave_monitor(&m->m);
	
	//Posso consumare
		
    sleep(1);
    *valore = m->vettore[index];

    printf("Consumazione di tipo 1 (valore=%d, index=%d)\n", *valore, index);
    
    //A questo punto rientro nel monitor
    enter_monitor(&m->m);
    
    	//Sono dentro. Aumento il numero di liberi e setto a libero statoindice
    	m->stato[index] = LIBERO;
    	m->num_liberi ++;
    	
    	//Segnalo i produttori di tipo 1 che possono produrre
    	signal_condition(&m->m,CV_PROD1);
    	
    //Posso uscire dal monitor
    leave_monitor(&m->m);
    
}


void consuma_tipo_2(MonitorPC * m, int * valore) {

    int index = 0;

    /* TBD: Aggiungere la sincronizzazione, 
        con la selezione dell'indice su cui consumare
        (alg. con vettore di stato) */
	
	//Per prima cosa entro nel monitor
	enter_monitor(&m->m);
	
		//Sono dentro. Check sulla condvar
		while (m->num_occupati_tipo2 == 0){
			printf("[CONS2] No tipo2, vado in wait\n");
			wait_condition(&m->m,CV_CONS2);
			printf("[CONS1] Risvegliato\n");
		}
		
		//A questo punto cerco un occupato1
		while (index < DIM && m->stato[index] != OCCUPATO2){
		
			//Fintantokè sto nel vettore e non trovo una casella occupata1 vado avanti
			index++;
			
		}
		
		printf("[CONS2] Trovata casella nella quale posso consumare\n");
		
		//A questo punto setto la casella a inuso e decremento occ di tipo 1
		m->stato[index] = INUSO;
		m->num_occupati_tipo2--;
		
	//Lascio il monitor
	leave_monitor(&m->m);
	
	//Posso consumare
		
    sleep(1);
    *valore = m->vettore[index];

    printf("Consumazione di tipo 2 (valore=%d, index=%d)\n", *valore, index);
    
    //A questo punto rientro nel monitor
    enter_monitor(&m->m);
    
    	//Sono dentro. Aumento il numero di liberi e setto a libero statoindice
    	m->stato[index] = LIBERO;
    	m->num_liberi ++;
    	
    	//Segnalo i produttori di tipo 1 che possono produrre
    	signal_condition(&m->m,CV_PROD2);
    	
    //Posso uscire dal monitor
    leave_monitor(&m->m);
}
