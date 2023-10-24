all: exe

processi-mutua-esclusione.o: processi-mutua-esclusione.c processini.h semafori.h
	gcc -c processi-mutua-esclusione.c

semafori.o: semafori.c semafori.h
	gcc -c semafori.c

main.o: main.c processini.h semafori.h
	gcc -c main.c

exe: main.o processi-mutua-esclusione.o semafori.o
	gcc -o main main.o processi-mutua-esclusione.o semafori.o

clean:
	rm -f *.o
	rm -f ./main
