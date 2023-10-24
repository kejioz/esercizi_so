#include "semafori.h"
#include "monitor_hoare.h"

#define CV_LETTORE 0 //CV sulla quale deve attendere il lettore, ovvero che non ci siano scrittori
#define CV_SCRITTORE 1 //CV sulla quale deve attendere il lettore, ovvero che non ci siano scrittori o lettori

#define MUTEXL 0 //Mutex per l'accesso alla variabile numero_lettori (serve sia per starv uno che entrambi)
#define SYNCH 2 //Mutex per la mutua esclusione tra lettori e scrittori (e per starv entrambi anche per soli scrittori)
#define MUTEXS 2 //Mutex per l'accesso alla variabile numero_scrittori ( serve solo per starv entramib)
#define MUTEX 3 //Mutex per la concorrenza tra scrittori che vogliono scrivere

typedef struct {

	int valore;
	
	int numero_lettori;

}LettScrittSemaforiUno;

typedef struct {

	int valore;
	
	int numero_lettori;
	int numero_scrittori;

}LettScrittSemaforiEntrambi;

typedef struct {

	int valore;
	
	int numero_lettori;
	int numero_scrittori;
	
	Monitor m;

}LettScrittMonitorEntrambi;

//Starvation scrittore sem
void lettoreSemaforiUno(int id_sem, LettScrittSemaforiUno* ls);
void scrittoreSemaforiUno(int id_sem, LettScrittSemaforiUno* ls);

//Starvation entrambi sem
void lettoreSemaforiEntrambi(int id_sem, LettScrittSemaforiEntrambi* ls);
void scrittoreSemaforiEntrambi(int id_sem, LettScrittSemaforiEntrambi* ls);

//Starvation entrambi monitor hoare
void lettoreMonitorEntrambi(LettScrittSemaforiEntrambi* ls);

//Starvation scrittore sem
void lettoreSemaforiUno(int id_sem, LettScrittSemaforiUno* ls){

	//Per prima cosa voglio mettermi in coda per leggere
	Wait_Sem(id_sem,MUTEXL);
		
		ls->numero_lettori++;
		
		//Se sono il primo lettore, allora devo fare una wait sul synch. Se è già bloccato c'è lo scrittore e devo aspettare, altrimenti blocco io gli scrittori
		if (ls->numero_lettori == 1){
			Wait_Sem(id_sem,SYNCH);
		}
		
	//Devo rilasciare l'accesso alla variabile numero lettori per permettere ad altri lettori di leggere
	Signal_Sem(id_sem,MUTEXL);
		
		//A questo punto sono dentro. Posso effettuare la lettura
		printf("[LETTORE] Sto leggendo %d\n",ls->valore);
		
	//Ora voglio riaccedere al numero di lettori
	Wait_Sem(id_sem,MUTEXL);
	
		//Lo decremento, siccome sto uscendo
		ls->numero_lettori--;
		
		//Sono l'ultimo lettore? Se sì, devo sbloccare gli scrittori
		if (ls->numero_lettori == 0){
			Signal_Sem(id_sem,SYNCH);
		}
		
	//Rilascio il semaforo x numero lettori
	Signal_Sem(id_sem,MUTEXL);

}

void scrittoreSemaforiUno(int id_sem,LettScrittSemaforiUno* ls){

	//Controllo. Posso entrare ? Lo chiedo al semaforo Synch. GLi chiedo : ci sono lettori? Se non ci sono lettori allora blocco tutto 
	Wait_Sem(id_sem,SYNCH);
	
		ls->valore = rand() % 5;
		
	//Ho finito. Che qualcuno si prenda synch, che sia un altro scrittore o i lettori
	Signal_Sem(id_sem,SYNCH);

}

//Starvation entrambi sem
void lettoreSemaforiEntrambi(int id_sem, LettScrittSemaforiEntrambi* ls){

	//Per prima cosa voglio mettermi in coda per leggere
	Wait_Sem(id_sem,MUTEXL);
		
		ls->numero_lettori++;
		
		//Se sono il primo lettore, allora devo fare una wait sul synch. Se è già bloccato c'è lo scrittore e devo aspettare, altrimenti blocco io gli scrittori
		if (ls->numero_lettori == 1){
			Wait_Sem(id_sem,SYNCH);
		}
		
	//Devo rilasciare l'accesso alla variabile numero lettori per permettere ad altri lettori di leggere
	Signal_Sem(id_sem,MUTEXL);
		
		//A questo punto sono dentro. Posso effettuare la lettura
		printf("[LETTORE] Sto leggendo %d\n",ls->valore);
		
	//Ora voglio riaccedere al numero di lettori
	Wait_Sem(id_sem,MUTEXL);
	
		//Lo decremento, siccome sto uscendo
		ls->numero_lettori--;
		
		//Sono l'ultimo lettore? Se sì, devo sbloccare gli scrittori
		if (ls->numero_lettori == 0){
			Signal_Sem(id_sem,SYNCH);
		}
		
	//Rilascio il semaforo x numero lettori
	Signal_Sem(id_sem,MUTEXL);

}

void scrittoreSemaforiEntrambi(int id_sem, LettScrittSemaforiEntrambi* ls){

	//Voglio scrivere anche io !
	Wait_Sem(id_sem,MUTEXS);
	
		ls->numero_scrittori++;
		
		//Sono il primo scrittore? Se sì, allora vedo se ci sono lettori. Se ci sono aspetto, ma appena finiscono, che tutti vengano bloccati!
		if (ls->numero_scrittori == 1){
			Wait_Sem(id_sem,SYNCH);
		}
	
	//Prendetevi la variabile numero scrittori
	Signal_Sem(id_sem,MUTEXS);
		
		//Ok con i lettori, ma ora devo vedermela con gli scrittori. È il mio turno?
		Wait_Sem(id_sem,MUTEX);
		
		//Finalmente posso scrivere!
		ls->valore = rand() % 5;
		
		//Signori scrittori, io ho finito, venite voi se volete
		Signal_Sem(id_sem,MUTEX);
		
	//Io però voglio notificare che ho finito eh!
	Wait_Sem(id_sem,MUTEXS);
		
		ls->numero_scrittori--;
		
		//Ma sono l'ultimo? Se sono l'ultimo allora devo avvisare i lettori
		if (ls->numero_scrittori == 0){
			Signal_Sem(id_sem,SYNCH);
		}
		
	//Ripijateve sto semaforo!
	Signal_Sem(id_sem,MUTEXS);
	
}

//Starvation entrambi MONITOR hoare
void lettoreMonitorEntrambi(LettScrittMonitorEntrambi* ls){

	//Come prima cosa entro nel monitor
	enter_monitor (&ls->m);
	
		//Sono un lettore, posso stare qui? Ovvero, mica ci sono scrittori? Check su cv RCORDANDO che è signal and wait, di conseguenza quiando sono segnalato subito riprendo esecuzione
		if (ls->numero_scrittori>0){
			wait_condition(&ls->m,CV_LETTORI);
		}
		
		//A questo punto so che non ci sono scrittori, cosa devo fare? Devo effettivamente aumentare il numero di lettori, ma soprattutto ogni lettore deve dire ad un altro che può leggere, siccome ho un signal and wait
		ls->numero_lettori++;
		
		//Segnalo un altro lettore
		signal_conditon(&ls->m,CV_LETTORI);
		
	//Posso uscire dal monitor
	leave_monitor(&ls->m);
		
		//Lettura, operaszione lenta
		sleep(1);
		
		printf("[LETTORE] Ho letto %d\n",ls->valore);
		
	//A questo punto ho letto, voglio rientrare nel monitor
	enter_monitor (&ls->m);
	
		//Voglio dire che ho finito di leggere
		ls->numero_lettori--;
		
		//Sono l'ultimo lettore?
		if (ls->numero_lettori == 0){
			
			//Se sì segnalo gli scrittori
			signal_condition(&ls->m,CV_SCRITTORI);
			
		}
		
	//Esco dal monitor
	leave_monitor(&ls->m);
		
}

void scrittoreMonitorEntrambi(LettScrittMonitorEntrambi* ls){

	//Entro nelm onitor
	enter_monitor(&ls->m);
	
		//CHJeclk sulla cv
		if (ls->numero_scrittori >0  || ls->numero_lettori >0){
			wait_condition(&ls->m,CV_SCRITTORI);
		}
		
		//Aumento numero scrittori
		ls->numero_scrittori ++;
		
	//Lascio il monitor
	leave_monitor(&ls->m);
	
	//Posso effettuare la scrittura
	ls->valore = rand() % 5);
	
	//Rientro nel monitor
	enter_monitor(&ls->m);
	
		//Voglio decrementare numero scrittori
		ls->numero_scrittori--;
		
		//CI sono scrittori in attesa sulla queue? Se sì segnalo loro, altrimenti segnalo un lettore
		if (queue_condition(&ls->m,CV_SCRITTORI){
			signal_condition(&ls->m,CV_SCRITTORI);
		}else{
			signal_codntiion(&ls->m,CV_LETTORI);
		}
		
	//Esco
	leave_monitor(&ls->m);

}

		
