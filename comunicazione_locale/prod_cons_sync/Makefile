all: main

main: p_c_msg.o procedure.o
	gcc -o main p_c_msg.o procedure.o

p_c_msg.o: p_c_msg.c header.h
	gcc -c p_c_msg.c

procedure.o: procedure.c header.h
	gcc -c procedure.c

clean:
	rm -f *.o
	rm -f ./main
