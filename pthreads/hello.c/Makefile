all: main

main: main.o
	gcc -pthread -o main main.o

main.o: main.c
	gcc -pthread -c main.c

clean:
	rm *.o
	rm ./main
