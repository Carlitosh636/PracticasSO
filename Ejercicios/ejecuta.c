#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char **argv){
	
	//int pip[2];
	int pid;
	int status;
	//pipe(pip);
	pid=fork();
	
	if(pid==0){
		/*close(pip[0]);
		close(pip[1]);*/
		execvp(argv[1],argv+1);
		fprintf(stderr,"%s: No se encuentra el mandato especificado\n",argv[1]);
	}
	else{
		/*close(pip[0]);
		close(pip[1]);*/
		wait(&status);
		if(WIFEXITED(status)!=0){
			if(WEXITSTATUS(status)!=0){
				fprintf(stderr,"%s: Error al ejecutar el mandato\n",argv[1]);
			}
		}
	
	}
	
}
