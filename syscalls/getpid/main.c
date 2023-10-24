#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]){

	int pid,ppid;
	
	pid = getpid(); //Mi ritorna il process id
	
	printf("Sono il processo con pid %d\n",pid);
	
	ppid = getppid(); //Mi ritorna il parent process id
	
	printf("Il mio processo padre è %d\n",ppid);
	
	return 0;

}
