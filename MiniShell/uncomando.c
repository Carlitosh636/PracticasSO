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
	int fichero;

	
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
				if(line -> redirect_input != NULL){ //si hay redirreccion de entrada
				
					fichero = open(line -> redirect_input, O_RDONLY);
					if(fichero == -1){
						fprintf(stderr, "i%: Error. Fallo al abrir el fichero de redireccion de entrada\n", fichero);
						exit(1);
					}else{
						dup2(fichero,0); // redirigimos la entrada
					}
					
					int fichaux;

					if(line -> redirect_ooutput != NULL){ // si hay redireccion de salida
	
						fichaux = open(line -> redirect_output, O_WRONLY | O_CREAT | O_TRUNC);
						if(fichaux == -1){
							fprintf(stderr, "%i: Error. Fallo al abrir el fichero de redireccion de salida \n", fichaux);
							exit(1);
						}else{
							dup2(fichaux,1);  //redirigimos la salida
						}
						
						execvp(line -> commands[0].filename, line -> commands[0].argv); // ejecutamos el comando
						fprintf(stderr, "%s: No se encuentra el mandato\n", line -> commands[i].filename);


						
					}else if(line -> redirect_error != NULL){  // si hay redireccion de error

						fichaux = open(line -> redirect_error, O_WRONLY | O_CREAT  | O_TRUNC );
						if(fichaux == -1){
							fprintf(stderr, "%i: Error: Fallo al abrir el fichero de redireccion de error\n", fichaux);
							exit(1);
						}
						
						dup2(fichaux,2);	//redirigimos la sallida de error
						execvp(line -> commands[0].filename, line -> commands[0].argv); 	//ejecutamos el comando
						fprintf(stderr, "%s: No se encuentra el mandato\n", line -> commands[i].filename);



					}else{ // si solo hay redireccion de  entrada 
						
						execvp(line -> commands[0].filename, line -> commands[0].argv); 	//ejecutamos el comando
						fprint(stderr, "%s: No se encuentra el mandato\n", line -> commands[i].filename);
						exit(1);
					}



				}else if(line -> redirect_output != NULL){ // si solo  hay redirecion solo  de salida 

					fichero = open(line -> redirect_output, O_WRONLY | O_CREAT | O_TRUNC );
					
					if(fichero == -1){
						fprintf(stderr, "%i: Error. Fallo al abrir el fichero de redireccion solo de salida", fichero);
						exit(1);
					}else{
						dup2(fichero,1); //redirigimos la salida
					}

					execvp(line -> commands[0].filename, line -> commands[0].argv); // ejecutamos el comando 
					fprintf(stderr, "%s: Error. No se encuentra el mandato\n", line -> commands[i].filename);
				
				}else if(line -> redurect_error != NULL){ // si hay redireccion de error 

					fichero = open(line -> redirect_error, O_WRONLY | O_CREAT | O_TRUNC  );
					
					if( fichero == -1){
						fprintf(stderr,"%i: Error. Fallo al abrir el fichero de redireccion de  entrada\n", fichero);
						exit(1);
					}else {
						dup2(fichero,2); // redirigimos la salida de error
					}
					
					execvp(line -> commands[0].filename, line -> commands[0].argv);
					fprintf(stderr, "%s:No se encuentra el mandato \n", line -> commands[i].filename);
					
				}else{ // si no hay redirecciones
					
					execvp(line -> commands[0].filename, line -> commands[0].argv);
					//si llega hasta aqui es que se ha producido un error en el execvp 
					fprintf("Error al ejecutar el comando: %s\n", strerror(errno));
					exit(1);
				}

			

			}else{ //Proceso padre
				 //waitpid(pid, NULL, 0);  prefiero la opcion  con status
				wait(&status);
				if(WIFEXITED(status) != 0 )
					if(WEXITSTATUS(status) != 0 )
						printf("El comando no se ejecuto correctamente \n");
				exit(0);
			}
		}
	}
}
