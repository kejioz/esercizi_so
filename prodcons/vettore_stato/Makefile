all: main

main: main.o procedure.o semafori.o
	gcc -o main main.o procedure.o semafori.o

semafori.o: semafori.c semafori.h
	gcc -c semafori.c

procedure.o: procedure.c procedure.h semafori.h
	gcc -c procedure.c

main.o: main.c procedure.h semafori.h
	gcc -c main.c


clean:
	rm *.o
	rm ./main
