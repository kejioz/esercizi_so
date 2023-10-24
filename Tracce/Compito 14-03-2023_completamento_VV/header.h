
#define NUM_OPERAZIONI 5
#define NUM_LETTORI 4

#define MUTEXL1 0
#define SYNCH1 1
#define MUTEXL2 2
#define SYNCH2 3

#define FTOK_PATH "."
#define FTOK_CHAR_CODA 'a'


typedef struct {
	int id_volo;
	int gate;
	
	//TODO: completare con le variabili per la sincronizzazione
	int numero_lettori;
	
} Info_Volo;

typedef struct {
	long volo;
	int gate;	
} msg;

void Wait_Sem(int, int );
void Signal_Sem (int, int );
void Lettore(Info_Volo* volo,int coda,int id_sem);
void Scrittore(Info_Volo* volo1,Info_Volo* volo2,int id_sem);
