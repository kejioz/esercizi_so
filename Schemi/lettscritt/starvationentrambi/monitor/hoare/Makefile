all: main

main: main.o procedure.o monitor_hoare.o
	gcc -o main main.o procedure.o monitor_hoare.o

main.o:	main.c
	gcc -c main.c

procedure.o: header.h procedure.c
	gcc -c procedure.c

monitor_hoare.o: monitor_hoare.h monitor_hoare.c
	gcc -c monitor_hoare.c

clean:
	rm *.o
	rm main
