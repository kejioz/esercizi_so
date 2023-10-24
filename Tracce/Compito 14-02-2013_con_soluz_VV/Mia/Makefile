all: main studente docente

semafori.o: semafori.h semafori.c
	gcc -c semafori.c

procedure.o: procedure.c procedure.h semafori.h
	gcc -c procedure.c

docente.o: docente.c procedure.h
	gcc -c docente.c

studente.o: studente.c procedure.h
	gcc -c studente.c

docente: docente.o procedure.o semafori.o
	gcc -o docente docente.o procedure.o semafori.o

studente: studente.o procedure.o semafori.o
	gcc -o studente studente.o procedure.o semafori.o

main.o: main.c procedure.h
	gcc -c main.c

main: main.o semafori.o procedure.o
	gcc -o main main.o semafori.o procedure.o

clean:
	rm *.o
	rm main studente docente
