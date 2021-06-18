#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char **argv){
	
	int pipEnviar[2];
	int pipRecibir[2];
	int pid;
	char *mensaje;
	char buf[1024];
	char bufAux[1024];
	int status;
	pipe(pipEnviar);
	pipe(pipRecibir);
	pid=fork();
	
	if(pid==0){
		close(pipRecibir[0]);	
		close(pipEnviar[1]);
		printf("%s\n","El hijo lee el mensaje");
		
		read(pipEnviar[0],bufAux,1024);
		for(int i =0;i<=strlen(bufAux);i++){
			bufAux[i]=toupper(bufAux[i]);
		}
		printf("%s\n","Paso el mensaje a padre");
		write(pipRecibir[1],bufAux,1024);
		close(pipRecibir[1]);	
		close(pipEnviar[0]);
		exit(0);
		
	}
	else{
		close(pipRecibir[1]);	
		close(pipEnviar[0]);	
		
		mensaje=argv[1];
		printf("%s\n","%sEl padre ha recogido el mensaje",mensaje);
		write(pipEnviar[1],mensaje,1024);
		printf("%s\n","Envio el mensaje");
		waitpid(pid,&status,0);
		printf("%s\n","Padre ha recibido el mensaje en mayus");
		read(pipRecibir[0],buf,1024);
		printf("%s\n","Printeo el mensaje en mayus");
		printf("%s\n",buf);
		close(pipRecibir[0]);	
		close(pipEnviar[1]);
		
	
	}
	
}
