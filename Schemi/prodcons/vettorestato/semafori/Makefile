all: main

main: main.o procedure.o semafori.o
	gcc -o main main.o procedure.o semafori.o

main.o:	main.c
	gcc -c main.c

procedure.o: header.h procedure.c
	gcc -c procedure.c

semafori.o: semafori.h semafori.c
	gcc -c semafori.c

clean:
	rm *.o
	rm main
