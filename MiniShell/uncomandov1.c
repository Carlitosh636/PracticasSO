#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include "parser.h"

int main(void){

	char buf[1024];
	tline *line;
	int pid;
	int status;

	
	printf("$:");

	while(fgets(buf, 1024, stdin)){
		
		line = tokenize(buf);
		
		if(line == NULL)
			continue;	
		
		if(line -> ncommands  == 1){
			pid = fork();
			
			if(pid < 0){ // ERROR

				fprintf(stderr, "Fallo el fork():\n%s\n", strerror(errno));
				exit(1);
			
			}else if (pid == 0){ //Proceso hijo

				execvp(line -> commands[0].filename, line -> commands[0].argv);
				
				//si llega aqui es que se ha producido un error en el execvp
				printf("Error al ejecutar el comando: %s\n", strerror(errno));
				exit(1);

			}else{ //Proceso padre
				
				wait(&status);
				if(WIFEXITED(status) != 0 )
					if(WEXITSTATUS(status) != 0 )
						printf("El comando no se ejecuto correctamente \n");
				exit(0);
			}
		}
	}
}
