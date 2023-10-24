all: start main_client main_server

start: main.c
	gcc -o start main.c
	
main_client: main_client.c header.h
	gcc -o main_client main_client.c
	
main_server: main_server.c header.h
	gcc -g -pthread -o main_server main_server.c
	
clean: 
	rm -f *.o
	rm -f main_client
	rm -f main_server
	rm -f start
