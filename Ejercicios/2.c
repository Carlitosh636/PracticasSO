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
	FILE *f1,*f2,*wr,*rd;
	char buf[1024];
	char bufAux[1024];
	int existencias[1024];
	int demandas[1024];
	int status;
	pipe(pip);
	pid=fork();
	int cont,cont2=0;
	int cambio=1;
	if(pid==0){
		close(pip[1]);
		rd=fdopen(pip[0],"r");
		while(fgets(bufAux,1024,rd)!=NULL){
			if(atoi(bufAux)<0){
				cambio=-1;
			}
			if(cambio>0){
				existencias[cont]=atoi(bufAux);
				cont++;
			}
			else{
				demandas[cont2]=atoi(bufAux);
				cont2++;
			}	
		}
		for(int i=0;i<cont;i++){
			printf("Se deben fabricar %i productos\n",existencias[i]-demandas[i]);
		}
		
		fclose(rd);
		close(pip[0]);
		exit(0);
		
	}
	else{
		close(pip[0]);
		f1=fopen(argv[1],"r");
		wr=fdopen(pip[1],"w");
		while(fgets(buf,1024,f1)!=NULL){
			fputs(buf,wr);
			fflush(wr);
		}
		fputs("-1",wr);
		fflush(wr);
		f2=fopen(argv[2],"r");
		while(fgets(buf,1024,f2)!=NULL){
			fputs(buf,wr);
			fflush(wr);
		}
		fclose(f1);
		fclose(f2);
		fclose(wr);
		close(pip[1]);
		exit(0);
		
	}
	return 0;
	
}
