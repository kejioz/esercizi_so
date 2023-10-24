all: master scrittore lettore

master: master.o procedure.o semafori.o
	gcc -o master master.o procedure.o semafori.o

scrittore: scrittore.o procedure.o semafori.o
	gcc -o scrittore scrittore.o procedure.o semafori.o

lettore: lettore.o procedure.o semafori.o
	gcc -o lettore lettore.o procedure.o semafori.o

master.o: master.c header.h
	gcc -c master.c

scrittore.o: scrittore.c header.h
	gcc -c scrittore.c

lettore.o: lettore.c header.h
	gcc -c lettore.c

semafori.o: semafori.c semafori.h
	gcc -c semafori.c

procedure.o: procedure.c header.h
	gcc -c procedure.c

clean:
	rm *.o
	rm master lettore scrittore
