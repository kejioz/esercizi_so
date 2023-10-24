#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

//Funzione da dichiare void* da passare al thread
void *PrintHello(void *threadid){

    //Nota che anche l'argomento è un void*, da castare poi opportunamente'
    printf("\n%ld: Hello World!\n",(long)threadid);
    pthread_exit(NULL);

}

int main(int argc, char* argv[]){

    //Creo un array di threads di TIPO PTHREAD_T
    pthread_t threads[NUM_THREADS];

    int rc; //Return code
    long t;

    for (t= 0;t<NUM_THREADS;t++){

        printf("Creating thread %ld\n", t);

        //Gli passo l'indirizzo del pthread_t, il secondo argomento sono gli attributi
        //Poi gli passo la procedura da eseguire, e gli attributi sempre void*'
        rc = pthread_create(&threads[t],NULL, PrintHello, (void *)t);

        //Se rc == 0 allora andato a buon fine

        if (rc == 0){

            printf("Pthread %ld andata a buon fine\n",t);

        }else{

            printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);

        }




    }

    //Se il mainthread usa pthread_exit, allora allowa i thjread figli a continuare esecuzione
    //Se invece chiama la exit() li killa
    pthread_exit(NULL);


}
