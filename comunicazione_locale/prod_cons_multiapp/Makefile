all: main p1 p2 p3

main: main.o procedure.o
	gcc -o main main.o procedure.o

main.o: main.c header.h
	gcc -c main.c

procedure.o: procedure.c header.h
	gcc -c procedure.c

p1: p1.o procedure.o
	gcc -o p1 p1.o procedure.o

p2: p2.o procedure.o
	gcc -o p2 p2.o procedure.o

p3: p3.o procedure.o
	gcc -o p3 p3.o procedure.o

p1.o: p1.c header.h
	gcc -c p1.c

p2.o: p2.c header.h
	gcc -c p2.c

p3.o: p3.c header.h
	gcc -c p3.c

clean:
	rm -f *.o
	rm -f ./main ./p1 ./p2 ./p3
