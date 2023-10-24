#ifndef _HEADER_H_
#define _HEADER_H_

#define REQUEST_TO_SEND 1
#define OK_TO_SEND 2
#define MESSAGGIO 3

typedef char msg[40];

typedef struct{
	
	long tipo;
	msg mess;

} Messaggio;

void Produttore(int queue,char* m);
void Consumatore(int queue);

void ReceiveBloc (Messaggio *,int,int);
void SendSincr (Messaggio*, int);
void initServiceQueues();
void removeServiceQueues();

#endif
