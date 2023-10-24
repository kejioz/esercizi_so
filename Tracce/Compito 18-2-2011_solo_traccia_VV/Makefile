all: main visualizzatore

main: main.o procedure.o semafori.o
	gcc -o main main.o semafori.o procedure.o

main.o: header.h main.c
	gcc -c main.c

procedure.o: header.h procedure.c
	gcc -c procedure.c

semafori.o: semafori.c semafori.h
	gcc -c semafori.c

visualizzatore.o: visualizzatore.c header.h
	gcc -c visualizzatore.c

visualizzatore: visualizzatore.o procedure.o semafori.o
	gcc -o visualizzatore visualizzatore.o procedure.o semafori.o

clean:
	rm *.o
	rm main
