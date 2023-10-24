#include <pthread.h>


typedef struct{

	//Dato
	int valore;
	
	//Variabili x sincronizzazione
	int numero_lettori;
	int numero_scrittori;
	int scrittori_attesa;


	//Mutex
	pthread_mutex_t mutex;
	
	//Cv
	pthread_cond_t cv_lett;
	pthread_cond_t cv_cons;

}LettScritt;

void* Lettore(void* ls);
void* Scrittore(void* ls);

void* Lettore(void* ls){

	//Casting
	ls = (LettScritt*) ls;
	
	//Entro monitor
	pthread_mutex_lock(&ls->mutex);
	
		//Check cv con while sig and continue
		while (ls->numero_scrittori > 0){
			
			//Wait
			pthread_cond_wait(&ls->cv_lett,&ls->mutex);
		
		}
		
		//Aum
		ls->numero_lettori++;
		
	//Esco
	pthread_mutex_unlock(&ls->mutex);
	
		//Lett
		printf("[L] letto %d\n",ls->valore);
		
	//Monit
	pthread_mutex_lock(&ls->mutex);
	
		ls->numero_lettori--;
		
		if (ls->numero_lettori == 0){
			
			pthread_cond_signal(&ls->cv_cons);
		
		}
		
	pthread_mutex_unlcok(&ls->mutex);

}

void* Scrittore(void* ls){

	//Casting
	ls = (LettScritt*) ls;
	
	//Entro monitor
	pthread_mutex_lock(&ls->mutex);
	
		//Check cv
		while (ls->numero_scrittori>0 || ls->numero_lettori>0){
			ls->scrittori_attesa++;
			pthread_cond_wait(&ls->cv_scrittori,&ls->mutex);
			ls->scrittori_attesa--;
		}

		ls->numero_scrittori++;
		
	//Lascio mon
	pthread_mutex_unlock(&ls->mutex);
	
		//Scritt
		ls->valore = rand() % 50;
		
	//Mon
	pthread_mutex_lock(&ls->mutex);
	
		//Decrement
		ls->numero_scrittori--;
		
		if (ls->scrittori_attesa > 0){
			pthread_cond_signal(&ls->cv_scrittori);
		}else{
			pthread_cond_broadcast(&ls->cv_lettori);
		}

	//Esco
	pthread_mutex_unlock(&ls->mutex);


}
