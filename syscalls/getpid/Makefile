GCC = gcc

all: main

main: main.o
	$(GCC) -o main main.o

main.o: main.c
	$(GCC) -c main.c

clear:
	rm -f *.o
	rm -f ./main
