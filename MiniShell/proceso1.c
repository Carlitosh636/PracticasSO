#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	if(argc>=1){
	int status=0;
	int pid=fork();
	if(pid<0){
		printf("Error al crear proceso hijo");
		exit(-1);
	}
	else if (pid==0){ //proceso hijo
		
		execvp(argv[1],argv+1); //lee primero el archivo en el que se transformará el hijo, y luego los parámetros del mandato (a partir de la pos 1, ya que tenemos )
		exit(0);
	}
	else{ //proceso padre
		wait(&status);
		if(WIFEXITED(status)){ //si hijo hizo un exit
			if(WEXITSTATUS(status)==0){
				printf("Error");
			}
		}
		exit(0);
	}
	}
	else{
		return -1;
	}
	
}
