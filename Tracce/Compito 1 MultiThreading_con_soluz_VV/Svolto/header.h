#include <pthread.h>
#define NUM_CLIENT 2

#define MSG_CLIENT 5
#define NUM_THREADS 10

#define TYPE 1

typedef struct {
	long type;
	int msg;
} messaggio;

typedef struct {
	pthread_mutex_t mutex;
	int buffer;
} thread_buffer;


	
