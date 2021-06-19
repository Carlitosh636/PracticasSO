#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main(int argc, char **argv){
	int pid1,pid2;
	int pip[2];
	pipe(pip);
	pid1=fork();
	pid2=fork(); //al tener más de 1 hijo y no usar bucles, lo que hacemos es una cadena de else ifs, y cada uno debe chekear si su pid respectivo es igual a 0. El ultimo será el padre
	/*printf("%s","Soy el proceso ");
	printf("%d\n",pid1);
	printf("%d\n",pid2);*/
	if(pid1==0 && pid2 != 0){
		close(pip[0]);		
		dup2(pip[1],1); //usaremos el write del pipe para meter cosas 
		execlp("ls","ls","-la",(char *) NULL); //en execlp tenemos que "repetir" el comando principal, seguido de los argumentos si es que hay, y al final siempre NULL
	}
	
	else if(pid1!=0 && pid2==0){
		close(pip[1]);
		dup2(pip[0],0); //recibo las vainas del pipe		
		execlp("grep","grep","my",(char *) NULL);
		
	}
	else if(pid1!=0 && pid2!=0){
		close(pip[0]);
		close(pip[1]);
		wait(NULL);		
		
	}
	return 0;
	
}
