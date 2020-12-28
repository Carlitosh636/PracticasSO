#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){
	int pid1,pid2,pipa[2];//solo los hijos se comunican entre si, necesitamos soolo 1 pipe.
	char buf[1024];
	pipe(pipa);
	pid1=fork();
	if(pid1<0){
		printf("Error al crear pipe");
	}
	else if(pid1==0){ //proceso hijo
		//recireccionar la salida estándar a pipa[1] para mandarselo al hijo 2 y luego hacemos el execlp(ls...)
		close(pipa[0]);
		dup2(pipa[1],1);
		execlp("ls","ls","-la",(char *)NULL);
		close(pipa[1]);
	}
	else{
		pid2=fork();
		if(pid2==0){
			close(pipa[1]);
			execlp("tr","tr","d","D",(char *)NULL);
			close(pipa[0]);
		}
		else{
			wait(NULL);
			wait(NULL);
			close(pipa[1]);
			while(read(pipa[0],buf,1)>0){
				write(1,buf,1);
			}		
			close(pipa[0]);
		}
	}
	return 0;	
}
