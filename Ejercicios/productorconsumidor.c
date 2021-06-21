#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char **argv){
	
	int pip[2];
	int nr;
	int pid;
	FILE *fEntrada,*fSalida,*aux,*aux2;
	char *ptr,*ptr2;
	char buf[1024];
	char bufAux[1024];
	int status;
	pipe(pip);
	pid=fork();
	int cont=1;
	if(pid==0){
		close(pip[1]);
		aux2=fdopen(pip[0],"r");
		fSalida=fopen(argv[2],"w"); //fichero para pegar cosas
		while((ptr2=fgets(bufAux,1024,aux2))!=NULL){
			if(atoi(bufAux)%2 ==0){
				fputs(bufAux,fSalida);
				fflush(fSalida);
			}
			
		}
		close(pip[0]);
		exit(0);
		
	}
	else{
		close(pip[0]);
		fEntrada=fopen(argv[1],"r"); //abrimos el archivo de lectura
		aux=fdopen(pip[1],"w"); //preparamos un puntero FILE para actuar como el pip[1]
		while((ptr=fgets(buf,1024,fEntrada))!=NULL){ //mientras no llegue a EOF
			fputs(buf,aux);
			fflush(aux); //copiamos y pegamos al final del pip[1]
			
		}
		wait(NULL);
		fclose(fEntrada);
		fclose(aux);
		close(pip[1]);
		exit(0);
		
	}
	return 0;
	
}
