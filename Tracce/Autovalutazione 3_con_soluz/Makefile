all: start

start: main.o procedure.o
	gcc -o start main.o procedure.o -pthread

main.o: main.c header.h
	gcc -c -o main.o main.c

procedure.o: procedure.c header.h
	gcc -c -o procedure.o procedure.c

clean:
	rm -f start
	rm -f *.o
