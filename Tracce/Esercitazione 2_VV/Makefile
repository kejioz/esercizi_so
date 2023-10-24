all: main

main: main.o procedure.o monitor_signal_continue.o
	gcc -o main main.o monitor_signal_continue.o procedure.o

main.o:	main.c header.h
	gcc -c main.c

procedure.o: header.h procedure.c monitor_signal_continue.h
	gcc -c procedure.c

monitor_signal_continue.o: monitor_signal_continue.c monitor_signal_continue.h
	gcc -c monitor_signal_continue.c

clean:
	rm *.o
	rm ./main
