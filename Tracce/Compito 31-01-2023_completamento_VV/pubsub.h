#ifndef _HEADER_
#define _HEADER_

#define TOPIC1 1
#define TOPIC2 2

#define NUM_PUBLISHERS_PER_TOPIC 1
#define NUM_SUBSCRIBERS_PER_TOPIC 2

#define NUM_MESSAGGI 3
#define TOTALE_SUBSCRIBER (NUM_PUBLISHERS_PER_TOPIC * 2)
#define TOTALE_MESSAGGI (NUM_PUBLISHERS_PER_TOPIC * 2 * NUM_MESSAGGI)

#define FTOK_PATH "."
#define FTOK_CHAR_CODA_MESSAGGI 'a'
#define FTOK_CHAR_CODA_REGISTRAZIONI 'b'


typedef struct {

    /* TBD: 
        Definire la struttura del messaggio, 
        includendo un topic (numerico)
        e un valore (numerico)
     */
     long type;
     int valore;

} messaggio_valore;

typedef struct {

    /* TBD: 
        Definire la struttura del messaggio, 
        includendo un topic (numerico)
        e lo ID della coda su cui ricevere i messaggi
     */
     long type;
     int id_coda;

} messaggio_registrazione;

#endif
