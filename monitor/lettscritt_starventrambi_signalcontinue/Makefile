all: main

main: main.o procedure.o monitor_signal_continue.o
	gcc -o main main.o procedure.o monitor_signal_continue.o

main.o: main.c procedure.h
	gcc -c main.c

procedure.o: procedure.h procedure.c monitor_signal_continue.h
	gcc -c procedure.c

monitor_signal_continue.o: monitor_signal_continue.c monitor_signal_continue.h
	gcc -c monitor_signal_continue.c


clean:
	rm *.o
	rm ./main
