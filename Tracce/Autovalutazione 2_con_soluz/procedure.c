#include "procedure.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <time.h>


float* leggi_risultati(struct MonitorRisultati * ls) {

	float* risultati = (float*) malloc(sizeof(float)*N_OPS);

	enter_monitor( &(ls->m) );

	printf("[%ld] Lettura - ingresso monitor\n", getpid());

	if( ls->numero_scrittori > 0 ) {

		printf("[%ld] Sospensione - lettura\n", getpid());
        // Nel caso signal_and_wait, lo scrittore segnala e va in attesa, e quindi può segnalare
        // solo un lettore invece di tutti come per la soluzione signal_and_continue.
		wait_condition( &(ls->m), VARCOND_LETTORI );
		printf("[%ld] Riattivazione - lettura\n", getpid());
	}

	ls->numero_lettori++;

	printf("[%ld]  Numero lettori ++ : %d\n", getpid(), ls->numero_lettori);

	/*
	 * NOTA: Ogni lettore riattiva il lettore successivo
	 * (il primo lettore attiva il secondo ancora in attesa;
	 * il secondo lettore attiva il terzo; etc.)
	 */

	signal_condition( &(ls->m), VARCOND_LETTORI );

	leave_monitor( &(ls->m) );

	sleep(1);

	for (int i = 0; i < N_OPS ; i++) {
		risultati[i] = ls->risultati[i];
		//printf("Lettura - valori [%f]\n", risultati[i]);
	}

	enter_monitor( &(ls->m) );

	ls->numero_lettori--;

	printf("[%ld] Numero lettori -- : %d\n",getpid(), ls->numero_lettori);


	/*
	 * NOTA: l'ultimo lettore ad abbandonare la risorsa
	 * riattiva uno degli scrittori in attesa
	 */

	if( ls->numero_lettori == 0 ) {

		printf("[%ld] Lettura - signal su scrittori\n", getpid());

		signal_condition( &(ls->m), VARCOND_SCRITTORI );
	}

	leave_monitor( &(ls->m) );

	printf("[%ld] Lettura - uscita monitor\n", getpid());

	return risultati;

}


void inserisci_risultato(struct MonitorRisultati * ls, float valore, int operazione) {

	enter_monitor( &(ls->m) );

	printf("[%ld] Scrittura - ingresso monitor\n", getpid());

	if(ls->numero_lettori > 0 || ls->numero_scrittori > 0) {

		printf("[%ld] Scrittura - sospensione\n", getpid());
		wait_condition( &(ls->m), VARCOND_SCRITTORI );
		printf("[%ld] Scrittura - riattivazione\n", getpid());
	}

	ls->numero_scrittori++;

	printf("[%ld] Numero scrittori ++ : %d\n", getpid(), ls->numero_scrittori);

	leave_monitor( &(ls->m) );


	// ...operazione lenta...
	sleep(1);

	ls->risultati[operazione%3] = valore;

	printf("[%ld] Scrittura - valore [%f] operazione %d\n", getpid(),valore, operazione);



	enter_monitor( &(ls->m) );

	ls->numero_scrittori--;

	printf("[%ld] Numero scrittori -- : %d\n", getpid(), ls->numero_scrittori);


	/*
	 * NOTA: per bilanciare la starvation, si verifica se ci
	 * sono scrittori in attesa, e se ne riattiva uno se presente.
	 * Altrimenti, si riattiva un lettore.
	 */

	if( queue_condition( &(ls->m), VARCOND_SCRITTORI ) ) {

		printf("[%ld] Scrittura - signal su scrittori\n", getpid());

		signal_condition( &(ls->m), VARCOND_SCRITTORI );

	} else {

		/*
		 * NOTA: si effettua una singola signal_cond e si riattiva
		 * un solo un processo lettore. Per questo motivo,
		 * il primo lettore a svegliarsi dovrà fare anche lui una
		 * signal_cond per riattivare il secondo lettore in attesa;
		 * il secondo lettore dovrà riattivare il terzo lettore; etc.
		 */

		printf("[%ld] Scrittura - signal su lettori\n", getpid());

		signal_condition( &(ls->m), VARCOND_LETTORI );
	}

	leave_monitor( &(ls->m) );

	printf("[%ld] Scrittura - uscita monitor\n", getpid());
}

void calcolo(struct MonitorRisultati * ls, int operazione, int mailbox){
	Messaggio m[2];
	float valore = 0;
	for (int i = 0 ; i < 2; i++){
		for (int j = 0 ; j < 2; j++){
			msgrcv(mailbox, (void *) &m[j],sizeof(Messaggio)-sizeof(long), operazione, 0);
			printf("[%ld] MESSAGGIO RICEVUTO: <%f>\n",getpid(), m[j].operando);
		}
		if (operazione == SOMMA)
			valore = m[0].operando + m[1].operando;
		else if (operazione == PRODOTTO)
			valore = m[0].operando * m[1].operando;
		else if (operazione == DIVISIONE)
			valore = m[0].operando / m[1].operando;
		else
			valore = -1;
		sleep(1);
		inserisci_risultato(ls, valore, operazione);
	}
}
void generazione(int operazione, int mailbox) {
	srand(time(NULL));
	Messaggio m;
	for (int i = 0 ; i < 4; i++) {
		m.tipo = operazione;
		m.operando = (float)(rand()%20 +1);
		// invio messaggio
		msgsnd(mailbox,(void*)&m,sizeof(Messaggio)-sizeof(long),0);
		printf("[%ld]MESSAGGIO INVIATO: <%f> OPERAZIONE %d\n",getpid(), m.operando, m.tipo);
	}
}
void printer(struct MonitorRisultati * ls) {
	float* risultati;
	for (int i = 0 ; i < 4; i++) {
		sleep(2);
		risultati = leggi_risultati(ls);
		printf("[%ld] Risultati 1: %f, 2: %f, 3: %f\n",getpid(), risultati[0], risultati[1], risultati[2]);
		free(risultati);
	}
}
