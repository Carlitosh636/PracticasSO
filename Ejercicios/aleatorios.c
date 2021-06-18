#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void sig_handler(int signum){
	switch(signum){
		case SIGUSR1:
			printf("%s\n","Soy la señal recibida por el hijo y te printearía un numero aleatorio");
			break;
		case SIGUSR2:
			exit(0);
			break;
		default:
			break;
			
	}
	
}

int main(int argc, char **argv){
	
	int pip[2];

	int pid;

	char buf[1024];

	int status;
	pipe(pip);
	pid=fork();
	signal(SIGUSR1,sig_handler);
	signal(SIGUSR2,sig_handler);
	if(pid==0){
		while(1);
		
	}
	else{
		while(fgets(buf,1024,stdin)){
			printf("%s","Enviando señal al hijo \n");
			kill(pid,SIGUSR1);
		}
		kill(pid,SIGUSR2);
		exit(0);
	
	}
	
}
