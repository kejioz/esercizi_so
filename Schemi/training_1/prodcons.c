#include "semafori.h"
#include "monitor_signal_continue.h"

#define DIM 4

#define CV_PRODUTTORI 0 //Per le condizioni dei monitor
#define CV_CONSUMATORI 1 //Per le condizioni dei monitor

#define MESSAGGI_DISPONIBILI 0 //Inizializzato a 0
#define SPAZIO_DISPONIBILE 1 //Inizializzato a DIM
#define MUTEX_P 2 //Inizializzato ad 1
#define MUTEX_C 3 //Inizializzato ad 1

#define LIBERO 0 //Per il vettore di stato
#define IN_USO 1 //Per il vettore di stato. Ricorda di rilasciare semaforo o monitor dopo aver settato a inuso
#define OCCUPATO 2 //Per il vettore di stato

typedef struct {

	int buffer[DIM];
	int testa;
	int coda;
	int elems;
	
}ProdConsCircSemafori;

typedef struct {

	int buffer[DIM];
	int testa;
	int coda;
	int elems;
	
	Monitor m;

}ProdConsCircMonitor;

typedef struct {

	int buffer[DIM];
	int stato[DIM];
	
}ProdConsStatoSemafori;

typedef struct {

	int buffer[DIM];
	int stato[DIM];
	
	Monitor m;
	
	//In questo caso aggiungo delle variabili ausiliarie per checkare le cv
	int numero_liberi;
	int numero_occupati;
	
}ProdConsStatoMonitor;

//Semafori CIRCOLARE
void produciCircSemafori(int id_sem,ProdConsCircSemafori* p);
void consumaCircSemafori(int id_sem,ProdConsCircSemafori* p);

//Semafori VETTSTATO
void produciStatoSemafori(int id_sem,ProdConsStatoSemafori* p);
void consumaStatoSemafori(int id_sem,ProdConsStatoSemafori* p);

//Monitor CIRCOLARE
void produciCircMonitor(ProdConsCircMonitor* p);
void consumaCircMonitor(ProdConsCircMonitor* p);

//Monitor VETTSTATO
void produciStatoMonitor(ProdConsStatoMonitor* p);
void consumaStatoMonitor(ProdConsStatoMonitor* p);

//Semafori CIRCOLARE
void produciCircSemafori(int id_sem,ProdConsCircSemafori* p){

	//Per prima cosa devo vedere se c'è spazio disponibile
	Wait_Sem(id_sem,SPAZIO_DISPONIBILE);
	
		//C'è spazio, posso produrre. Devo competere però con altri produttori
		Wait_Sem(id_sem,MUTEX_P);
		
			//E' il mio turno. Produco.
			p->buffer[p->coda] = rand() % 5;
			p->elems++;
			p->coda = (p->coda +1) % DIM;
			
		//Segnalo ad un altro produttore in atetsa che può produrre lui
		Signal_Sem(id_sem,MUTEX_P);
		
	//Segnalo ad un consumatore in attesa che può consumare
	Signal_Sem(id_sem,MESSAGGIO_DISPONIBILE);

}

void consumaCircSemafori (int id_sem, ProdConsCircSemafori* p){

	//Devo vedere se c'è un messaggio disponibiel da consumare
	Wait_Sem(id_sem, MESSAGGIO_DISPONIBILE);
	
		//C'è un messaggio disponibile, ma devo competere con altri consumatori per consumare eventualmente
		Wait_Sem(id_sem, MUTEX_C);
		
			//Consumo
			printf ("[CONSUMATORE] Consumato %d\n",p->buffer[p->testa]);
			p->elems--;
			p->testa = (p->testa + 1) % DIM;
			
		//Segnalo ad altri consumatori in attesa che possono consumare
		Signal_Sem(id_sem, MUTEX_C);
		
	//Segnalo ad un produttore in attesa che può produrre
	Signal_Sem (id_sem , SPAZIO_DISPONIBILE);
	
}

//Semafori STATO
void produciStatoSemafori(int id_sem, ProdConsStatoSemafori* p){

	//Per prima cosa controllo se c'è spazio disponibile per consumare
	Wait_Sem(id_sem,SPAZIO_DISPONIBILE);
	
		//Competo con altri produttori per acquisire momentaneamente l'accesso al vettore
		Wait_Sem(id_sem,MUTEX_P);
		
			//Scorro il vettore fin quando non trovo l'indice dove poter produrre
			int index = 0;
			
			while (index < DIM && p->stato[index] != LIBERO){
				index++;
			}
			
			//A questo punto so dove posso produrre. Quindi lo setto in uso 
			p->stato[index] = IN_USO;
			
		//A questo punto posso rilasciare l'accesso al vettore ad altri produttori
		Signal_Sem(id_sem, MUTEX_P);
		
		//Posso effettivamente effettuare la produzione ( operazione eventualmente lenta )
		sleep (1);
		p->buffer[index] = rand () % 5;
		
		//Setto la casella a OCCUPATO
		p->stato[index] = OCCUPATO;
		
	//Segnalo ai consumatori che c'è un messaggio
	Signal_Sem(id_sem,MESSAGGIO_DISPONIBILE);

}

void consumaStatoSemafori(int id_sem, ProdConsStatoSemafori * p){

	//PEr prima cosa mi assicuro che c'è un messaggio da poter consumare
	Wait_Sem (id_sem, MESSAGGIO_DISPONIBILE);
	
		//Se c'è, allora blocco momentaneamente il vettore
		Wait_Sem (id_sem, MUTEX_C);
		
			//A questo putno devo trovare dove poter consumare
			int index = 0;
			
			while (index < DIM && p->stato[index] != OCCUPATO){
				index++;
			}
			
			//Setto la casella INUSO
			p->stato[index] = IN_USO;
			
		//A questo punto posso segnalare ad altri consumatori che possono accedere al vetore
		Signal_Sem(id_sem,MUTEX_C);
		
		//Posso consumnare x cassi iemi
		printf("[CONSUMATORE] Consumato con successo %d\n",p->buffer[index]);
		
		//Adesso posso settare lo stato a LIBERO
		p->stato[index] = LIBERO;
		
	//Segnalo ai produttori che c'p spoazio
	Signal_Sem (id_sem, SPAZIO_DISPONIBILE);

}

//Monitor CIRCOLARE
void produciCircMonitor(ProdConsCircMonitor* p){

	//Devo ricordarmi che sto lavorando con i monitor. Quindi tendenzialmente la prima cosa che faccio ogni volta che entro è controllare la variaible condition. Molto spesso utilizzo delle variabili ausiliarie.
	
	//Per prima cosa entro nel monitor. Voglio fare il check sulla condizione altrimenti aspettare
	enter_monitor(&p->m);
	
		//Sono nel monitor. Voglio verificare la mia condizione di produzione. C'è spazio? Ricorda che è sig e continue
		while(p->elems == DIM){	//Se non ho spazio, mi sospendo sulla cv. Ricorda che uso while siccome ho un signal and continue, quando mi sveglio non è detto che qualcheduno non abbia corrotto la condizione
		
			wait_condition(&p->m,CV_PRODUTTORI); //Ricorda che si esce automaticamente dal monitor quando si va in wait
			// e ci si rientra quando si viene segnalati mi sa.
		}
		
		//Se sono qui, vuol dire che posso effettivamente effettuare la produzione
		p->buffer[p->coda] = rand() % 5;
		p->elems++;
		p->coda = (p->coda +1) % DIM;
		
		//Segnalo a coloro che sono sospesi su CV_CONSUMATORI che possono consumare
		signal_condition(&p->m,CV_CONSUMATORI);
		
	//Esco dal monitor
	leave_monitor(&p->m);
	
}

void consumaCircMonitor(ProdConsCircMonitor *p){

	//Entro nel monitor e checko la condizione
	enter_monitor (&p->m);
	
		//Posso consumare? Se dim è uguale ad elems allora devo aspettare che qualcuno produca
		while (p->elems == 0){
			wait_condition(&p->m,CV_CONSUMATORI);
		}	
		
		//Consumazione
		printf ("[CONSUMATORE] Consumato %d\n",p->buffer[p->testa]);
		p->elems--;
		p->testa = (p->testa + 1) % DIM;
		
		//Segnalo che possono produrre a quelli in attesa sulla cv
		signal_condition(&p->m,CV_PRODUTTORI);
		
	//Esco dal monitor
	leave_monitor(&p->m);

}

void produciStatoMonitor(ProdConsStatoMonitor *p){

	//Entro nel monitor
	enter_monitor(&p->m);
	
		//Devo checkare la condizione. Qual è la conduzione di produzione? Che ci sia almeno un LIBERO
		while (p->numero_liberi == 0){
			wait_condition(&p->m,CV_PRODUTTORI);
		}
		
		//Normale scorrimento per il while. Cerco il libero.
		int index = 0;
		while (index<DIM && p->stato[index] != LIBERO){
			index++;
		}
		
		//Ho trovato dove poter produrre. Allora che faccio? Setto a INUSO e diminuisco il numero di liberi
		p->stato[index] = IN_USO;
		p->numero_liberi--;
		
	//A questo punto DEVO rilasciare il monitor, In quanto mi sono assicurato che osno l'unico ad usare la risorsa
	leave_monitor(&p->m);
	
	//Posso produrre
	sleep (1);
	p->buffer[index] = rand () % 5;
	
	//A questo punto rientro nel monitor. Devo far notare che ora la casella è occupata ed il numero di occupati è aum
	enter_monitor(&p->m);
	
		p->buffer[index] = OCCUPATO;
		p->numero_occupati++;
		
		//Segnalo ai processi in attesa sulla CV_CONSUMATORI, siccome numero_occupati non è più zero
		signal_condition(&p->m,CV_CONSUMATORI);
		
	//Esco dal monitor
	leave_monitor(&p->m);

}

void consumaStatoMonitor(ProdConsStatoMonitor *p){
	
	//Entro nel monitor
	enter_monitor(&p->m);
	
		//Devo checkare la variabile condition. Qual è ? Che ci sia almeno un occupato
		while (p->numero_occupati == 0){
			wait_condition(&p->m,CV_CONSUMATORI);
		}
		
		//A questo punto normale scorrimento
		int index = 0;
		while (index<DIM && p->stato[index] != OCCUPATO){
			index++;
		}
		
		//Ho trovato dove posso consumare. Setto a IN_USO, e di conseguenza decremento numero occupati
		p->stato[index] = IN_USO;
		p->numero_occupati--;
		
	//A questo putno posso rilasciare il monitor, posso permettere ad altri consumatori di consumare
	leave_monitor(&p->m);
	
	//Effettuo la cosnumazione
	printf("[CONSUMATORE] Consumato con successo %d\n",p->buffer[index]);
	
	//Devo rientrare nel monitor, per rendere persistenti le mie modifiche
	enter_monitor(&p->m);
	
		//Adesso la casella non è più in uso ma LIBERO, ed aumento il numeor di liberi quindi
		p->stato[index] = LIBERO;
		p->numero_liberi++;
		
		//Segnalo ad eventuali produttori in attesa sulla cv produttori
		signal_condition(&p->m,CV_PRODUTTORI);
		
	//Esco dal monitor
	leave_monitor(&p->m);
	
}	
