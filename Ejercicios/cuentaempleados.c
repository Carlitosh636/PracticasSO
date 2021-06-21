#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
int main(int argc, char **argv){
	
	int pip[2];
	int pid;
	FILE *fd;
	char *mensaje=argv[1];
	char *command={"wc","-l"};
	char buf[2048];
	char bufAux[2048];
	int status;
	
	pipe(pip);
	pid=fork();
	
	if(pid==0){
		close(pip[1]);
		dup2(pip[0],0);
		execv(command[0],command);		
		
	}
	else{
		close(pip[0]);
		
		fd=fopen(mensaje,"r");
		
		while(fgets(buf,2048,fd)!=NULL){
			write(pip[1],buf,2048);
		}
		wait(NULL);
		fclose(fd);
		close(pip[1]);
	}
	return 0;
}
