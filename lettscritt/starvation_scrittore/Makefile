all: main

semafori.o: semafori.c semafori.h
	gcc -c  semafori.c

procedure.o: procedure.c semafori.h
	gcc -c  procedure.c

main.o: main.c procedure.h semafori.h
	gcc -c  main.c

main: main.o procedure.o semafori.o
	gcc -o main main.o procedure.o semafori.o

clear:
	rm *.o
	rm ./main
