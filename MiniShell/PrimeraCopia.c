#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include "parser.h"

int nuevo_proc(int in,int out,char *mand,char **args){
	pid_t pid;
	pid=fork();
	if(pid==0){
		if(in!=0){
			dup2(in,0);
			close(in);
		}
		if(out!=1){
			dup2(out,1);
			close(out);
		}
		execvp(mand,args);
	}
	return pid;
}

struct nodo{
	int pid;
	char *mandName;
	struct nodo *sig;
};
struct nodo *head=NULL;

void MostrarLista(){
	struct nodo *prim =head;
	
	while(prim!=NULL){
		printf("[%d] --> %s \n",prim->pid,prim->mandName);
		prim=prim->sig;
	}  
}

void InsertarPID(int p,char *name){
	if(head==NULL){
		head=(struct nodo*)malloc(sizeof(struct nodo));
		head->pid=p;
		//head->mandName=malloc(1024);
		head->mandName=name;
		head->sig=NULL;
		printf("%d",head->pid);
		printf("%s",head->mandName);
	}
	else{
		struct nodo *nuevo=head;
		while(nuevo->sig!=NULL){
			nuevo=nuevo->sig;
		}
		nuevo->sig=(struct nodo*)malloc(sizeof(struct nodo));
		nuevo->sig->pid=p;
		//nuevo->sig->mandName=malloc(1024);
		nuevo->sig->mandName=name;
		nuevo->sig->sig=NULL;
		printf("%d",nuevo->pid);
		printf("%s",nuevo->mandName);
	}
	
}
int main(void) {

	char buf[1024];
	tline * line;
	pid_t pid,pid2;
	
	int status;
	int fichero,fichaux,ficherr;
	int i,contProc;
	int in,ult, pipa[2];
	i=0;
	printf("$: ");
	
	while (fgets(buf, 1024, stdin)!=NULL){

		line = tokenize(buf);
		if(line == NULL){
			continue;
		}

		if(strcmp(line-> commands[0].argv[0], "cd")==0){
			
			if(line->commands[0].argv[1]==NULL){
				chdir(getenv("HOME"));
			}
			else{
				int dir=chdir(line->commands[0].argv[1]);
				if(dir==-1){
					printf("El directorio actual no existe: %s \n",line->commands[0].argv[1]);
				}
			}
		}
		else if(strcmp(line-> commands[0].argv[0], "jobs")==0){
			MostrarLista();
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
				
				if(line ->background==0){
					waitpid(pid,&status,0);
				}else{
					InsertarPID(pid,line->commands[0].filename);
				}
				
			}
		}
		else if(line -> ncommands >= 2){
				
				
				in=0;
				for(contProc=0; contProc<line->ncommands-1;contProc++){
					pipe(pipa);
					nuevo_proc(in,pipa[1],line->commands[contProc].filename,line->commands[contProc].argv);
					close(pipa[1]);
					in=pipa[0];
				}
				ult=fork();
				if(ult==0){
					dup2(in,0);
					execvp(line->commands[contProc].filename,line->commands[contProc].argv);
				}
				else{
					if(line->background==0){
						waitpid(ult,&status,0);
					}
					else{
						InsertarPID(nuevo_proc(in,pipa[1],line->commands[contProc].filename,line->commands[contProc].argv),line->commands[contProc].filename);
					}
				}
				
			}
			printf("$: ");
			
		}
	return 0;
	}

