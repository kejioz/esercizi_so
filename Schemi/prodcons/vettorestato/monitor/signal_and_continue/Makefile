all: main

main: main.o procedure.o monitor_signal_continue.o
	gcc -o main main.o procedure.o monitor_signal_continue.o

main.o:	main.c
	gcc -c main.c

procedure.o: header.h procedure.c
	gcc -c procedure.c

monitor_signal_continue.o: monitor_signal_continue.h monitor_signal_continue.c
	gcc -c monitor_signal_continue.c

clean:
	rm *.o
	rm main
