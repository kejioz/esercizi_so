#define NUM_UPDATES 3
#define MAX_VALUES 4
#define NUM_CONS 12

#define FTOK_PATH "."
#define FTOK_CHAR_RICHIESTE 'a'
#define FTOK_CHAR_RISPOSTE 'b'


#define TIPOMESS 1

//Messaggio di richiesta
typedef struct {
	long type;
} req; 

//Messaggio di risposta
typedef struct {
	long type;
	int value;
} res; 


