all: main

main: main.o procedure.o monitor_hoare.o
	gcc -o main main.o procedure.o monitor_hoare.o

main.o: main.c procedure.h
	gcc -c main.c

procedure.o: procedure.h procedure.c monitor_hoare.h
	gcc -c procedure.c

monitor_hoare.o: monitor_hoare.c monitor_hoare.h
	gcc -c monitor_hoare.c


clean:
	rm *.o
	rm ./main
