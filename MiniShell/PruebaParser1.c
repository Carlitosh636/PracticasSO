#include <stdio.h>
#include <unistd.h>
#include "parser.h"

int main(){

	char buf[1024];
	tline * line;
	int i,j;
	printf("$");
	while(fgets(buf,1024,stdin)){
		line=tokenize(buf);
		if(line==NULL){
			printf("Error, no hay linea");
			return -1;
		}
		else{
		for(i=0;i<line->ncommands;i++){
			printf("orden %d (%s):\n", i, line->commands[i].filename);
			for (j=0; j<line->commands[i].argc; j++) {
				printf("  argumento %d: %s\n", j, line->commands[i].argv[j]);
			}
		}
		execvp(line->commands[0].filename,line->commands[0].argv);
		}
	}
	return 0;
}
