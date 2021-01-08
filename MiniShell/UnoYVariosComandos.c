#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include "parser.h"

int main(void) {

	char buf[1024];
	tline * line;
	int pid,pid2;
	int *pids;
	int **pips;
	int status;
	int fichero,fichaux,ficherr;
	int i,contFinal,contPip,contProc;
	i=0;
	printf("$: ");
	
	while (fgets(buf, 1024, stdin)){

		line = tokenize(buf);
		if(line == NULL){
			continue;
		}

		

		else if(line->ncommands == 1){ //Si hay un solo comando
			
			pid = fork();

			if(pid < 0){  //Error en el fork

				fprintf(stderr, "Fallo el fork() %s/n" , strerror(errno));
				exit(1);

			} else if(pid == 0){ //si es el hijo

				if(line->redirect_input != NULL){ //si hay redirección de entrada
						
					
						printf("redirección de entrada: %s\n", line->redirect_input);
						fichero = open(line->redirect_input, O_RDONLY);
						
						if (fichero == -1) {
							fprintf(stderr,"%i: Error. Fallo al abrir el fichero de redirección de entrada\n", fichero);
							exit(1);
						} else {
							dup2(fichero,0); //redirijimos la entrada
						}

					

					if (line->redirect_output != NULL) { //si hay redirección de salida
						
						fichaux = open(line->redirect_output, O_WRONLY | O_CREAT | O_TRUNC , 0600);

						if (fichaux == -1) {
							fprintf(stderr,"%i: Error. Fallo al abrir el fichero de redirección de salida\n", fichaux);
							exit(1);
						} else {
							dup2(fichaux,1); //redirigimos la salida
						}

						execv(line->commands[0].filename, line->commands[0].argv); //ejecutamos el comando
						fprintf(stderr,"%s: No se encuentra el mandato\n",line->commands[i].filename);

					}else if (line->redirect_error != NULL) { //si hay redirección de error

						
						fichaux = open(line->redirect_error, O_WRONLY | O_CREAT | O_TRUNC , 0600);

						if (fichaux == -1){
							fprintf(stderr,"%i: Error. Fallo al abrir el fichero de redirección de error\n", fichaux);
							exit(1);
						}
						dup2(fichaux,2); //redirigimos la salida de error
						execv(line->commands[0].filename, line->commands[0].argv); //ejecutamos el comando
						fprintf(stderr,"%s: No se encuentra el mandato\n",line->commands[i].filename);

					} else { //si solo hay redirección de entrada
						execv(line->commands[0].filename, line->commands[0].argv); //ehecutamos el comando
						fprintf(stderr,"%s: No se encuentra el mandato\n",line->commands[i].filename);
					}

				} else if (line->redirect_output != NULL) { //si solo hay redirección de salida

				
					fichero = open(line->redirect_output, O_WRONLY | O_CREAT | O_TRUNC , 0600);

					if (fichero == -1) {
						fprintf(stderr,"%i: Error. Fallo al abrir el fichero de redirección de salida\n", fichero);
						exit(1);
					} else {
						dup2(fichero,1); //redirigimos la salida
					}

					execv(line->commands[0].filename, line->commands[0].argv); //ejecutamos el comando
					fprintf(stderr,"%s: No se encuentra el mandato\n",line->commands[i].filename);

				} else if (line->redirect_error != NULL) { //si solo hay redirección de error

					
					fichero = open(line->redirect_error, O_WRONLY | O_CREAT | O_TRUNC , 0600);

					if (fichero == -1) {
						fprintf(stderr,"%i: Error. Fallo al abrir el fichero de redirección de entrada\n", fichero);
						exit(1);
					} else {
						dup2(fichero,2); //redirigimos la salida de error
					}

					execv(line->commands[0].filename, line->commands[0].argv); //ejecutamos el comando
					fprintf(stderr,"%s: No se encuentra el mandato\n",line->commands[i].filename);

				} else { //si no hay redirecciones

					execv(line->commands[0].filename, line->commands[0].argv); //ejecutamos el comando
					fprintf(stderr,"%s: No se encuentra el mandato\n",line->commands[i].filename); 
					exit(1);
				}

			}else{ //el padre
				
				//espera al hijo
				wait(&status);
				if(WIFEXITED(status) != 0)
					if(WEXITSTATUS(status) != 0)
						printf("El comando no se ejecuto correctamente\n");
				
				exit(0);
			}
		}
		else if(line -> ncommands >= 2){
				pids=malloc(line -> ncommands * sizeof(int));
				pips=(int **) malloc((line -> ncommands - 1) * sizeof(int *));
				
				
				for(contProc=0; contProc < line -> ncommands-1; contProc++){
					pips[contProc]=(int *) malloc (2*sizeof(int));
					if(pipe(pips[contProc]) < 0){
						fprintf(stderr,"Fallo al crear el pipe %s \n",strerror(errno));
					}
				}
				for(contProc=0; contProc < line -> ncommands; contProc++){
					pid2=fork();
					if(pid2<0){
						fprintf(stderr,"Fallo el fork()");
						exit(-1);
					}
					else if(pid2==0){
						
						if(contProc==0){
							if(line->redirect_input != NULL){ //si hay redirección de entrada
						
					
								printf("redirección de entrada: %s\n", line->redirect_input);
								fichero = open(line->redirect_input, O_RDONLY);
						
								if (fichero == -1) {
									fprintf(stderr,"%i: Error. Fallo al abrir el fichero de redirección de entrada\n", fichero);
									exit(1);
								} else {
									dup2(fichero,0); //redirijimos la entrada
								}
							}	
						for(contPip=1; contPip<line -> ncommands-1;contPip++){
							close(pips[contPip][1]);
							close(pips[contPip][0]);
						}
						close(pips[0][0]);
						dup2(pips[0][1],1);
						}
					if(contProc>0 && contProc < line -> ncommands-1){
						if(contProc==1 && line -> ncommands != 3){
							for(contPip=contProc+1; contPip<line -> ncommands-1; contPip++){
								close(pips[contPip][1]);
								close(pips[contPip][0]);
							}
						}
						if(contProc==line -> ncommands-2 && line -> ncommands != 3){
							for(contPip=0; contPip<contProc-1; contPip++){
								close(pips[contPip][1]);
								close(pips[contPip][0]);
							}
						}
						if(contProc!=1 && contProc!=line -> ncommands-2 && line -> ncommands != 3){
							for(contPip=0; contPip<contProc-1; contPip++){
								close(pips[contPip][1]);
								close(pips[contPip][0]);
							}
							for(contPip=contProc+1; contPip<line -> ncommands-1; contPip++){
								close(pips[contPip][1]);
								close(pips[contPip][0]);
							}
						}
						close(pips[contProc-1][1]);
						dup2(pips[contProc-1][0],0);
						close(pips[contProc][0]);
						dup2(pips[contProc][1],1);
					}
					if(contProc== line -> ncommands-1){
						if (line->redirect_output != NULL) { //si hay redirección de salida
							fichaux = open(line->redirect_output, O_WRONLY | O_CREAT | O_TRUNC , 0600);
							if (fichaux == -1) {
								fprintf(stderr,"%i: Error. Fallo al abrir el fichero de redirección de salida\n", fichaux);
								exit(1);
							} else {
								dup2(fichaux,1); //redirigimos la salida
							}

						}
						if (line->redirect_error != NULL) { //si solo hay redirección de error
							ficherr = open(line->redirect_error, O_WRONLY | O_CREAT | O_TRUNC , 0600);
							if (ficherr == -1) {
								fprintf(stderr,"%i: Error. Fallo al abrir el fichero de redirección de entrada\n", ficherr);
								exit(1);
							} else {
								dup2(ficherr,2); //redirigimos la salida de error
							}
						}
						for(contPip=0;contPip<line->ncommands-2;contPip++){
							close(pips[contPip][1]);
							close(pips[contPip][0]);
						}
						close(pips[contProc-1][1]);
						dup2(pips[contProc-1][0],0);

					}
					execv(line->commands[contProc].filename, line->commands[contProc].argv);
					}
					else{
						pids[contProc]=pid2;
					}
				}
				for(contFinal=0; contFinal< line -> ncommands-1; contFinal++){
					close(pips[contFinal][1]);
					close(pips[contFinal][0]);
				}
				for(contFinal=0; contFinal< line -> ncommands-1; contFinal++){
					waitpid(pids[contFinal],NULL,0);
				}
				for(contProc=0; contProc>line->ncommands -1; contProc++){
					free(pips[contProc]);
				}
				free(pids);
				free(pips);
			}
			fprintf("$:");
			
		}

	return 0;
	}

