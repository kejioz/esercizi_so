all: main

main: main.o semafori.o
	gcc -o main main.o semafori.o

main.o: main.c semafori.h
	gcc -c main.c

semafori.o: semafori.c semafori.h
	gcc -c semafori.c

clear:
	rm -f *.o
	rm -f ./main
