#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include "parser.h"

//Función auxiliar para 2 o más procesos. toma valores para las redirecciones de entrada/salida de los procesos y los mandatos con argumentos que ejecutar
int nuevo_proc(int in,int out,char *mand,char **args){
	pid_t pid;
	pid=fork();
	if(pid==0){
		if(in!=0){
			//si es el primer mandato, no necesitamos lectura
			dup2(in,0);
			close(in);
		}
		//el resto es para comandos intermedios
		if(out!=1){
			dup2(out,1);
			close(out);
		}
		//ejecuta el mandato
		execvp(mand,args);
	}
	return pid;
}
/*Para saber que procesos están en BG necesitamos una lista, ya que no sabemos cuántos procesos se pondrán en BG. Con un nodo que contenga PID y nombre, y una función para insertar
y otra para mostrar todo, es suficiente.*/
struct nodo{
	pid_t pid;
	char *mandName;
	struct nodo *sig;
};
struct nodo *head=NULL;
struct nodo *current=NULL;

void MostrarLista(){
	struct nodo *prim =head;
	
	while(prim!=NULL){
		printf("[%d] --> %s \n",prim->pid,prim->mandName);
		prim=prim->sig;
	}  
}

void InsertarPID(pid_t p,char *name){
	if(head==NULL){
		head=(struct nodo*)malloc(sizeof(struct nodo));
		head->pid=p;
		head->mandName=name;
		head->sig=NULL;
		printf("%d",head->pid);
		printf("%s",head->mandName);
		current=head;
	}
	else{
		struct nodo *nuevo=head;
		while(nuevo->sig!=NULL){
			nuevo=nuevo->sig;
		}
		nuevo->sig=(struct nodo*)malloc(sizeof(struct nodo));
		nuevo->sig->pid=p;
		nuevo->sig->mandName=name;
		nuevo->sig->sig=NULL;
		printf("%d",nuevo->pid);
		printf("%s",nuevo->mandName);
		current=nuevo;
	}
	
}
int ReturnLastPID(){
	return (pid_t)current->pid;	

}
int main(void) {

	char buf[1024];//buffer entrada
	tline * line; //linea a parsear
	pid_t pid; //pid del fork
	
	int status;
	int fichero,fichaux,ficherr;//descr de ficheros
	int i,contProc;//contadores
	int in,ult, pipa[2];//pipe y variables para 2+ mandatos
	i=0;
	printf("$: ");
	
	while (fgets(buf, 1024, stdin)!=NULL){ //mientras se introduzcan cosas y no se detenga con CTRL+D

		line = tokenize(buf);
		if(line == NULL){ //si no se ha introducido
			continue;
		}

		if(strcmp(line-> commands[0].argv[0], "cd")==0){ //si el mandato es cd
			
			if(line->commands[0].argv[1]==NULL){
				chdir(getenv("HOME")); // si no tiene argumentos, se pasa al dir estandar $HOME
			}
			else{
				int dir=chdir(line->commands[0].argv[1]); //se lleva al dir indicado y si no existe, se avisa.
				if(dir==-1){
					printf("El directorio actual no existe: %s \n",line->commands[0].argv[1]);
				}
			}
		}
		else if(strcmp(line-> commands[0].argv[0], "jobs")==0){ //si es mandato jobs
			MostrarLista(); //función aux mostrar lista
		}
		else if(strcmp(line-> commands[0].argv[0], "fg")==0){ // si es mandato fg
			if(line->commands[0].argv[1]==NULL){
				waitpid(ReturnLastPID,&status,0); //se espera a que termine el ultimo proceso en BG
			}
			else{
				waitpid(atoi(line->commands[0].argv[1]),&status,0); //se espera a que termine el proceso entrado por param
			}
			
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
				
				//espera al hijo si es un proceso para FG. Si no, continúa ejecutando el código e introduce el PID en la lista
				
				if(line ->background==0){
					waitpid(pid,&status,0);
				}else{
					InsertarPID(pid,line->commands[0].filename);
				}
				
			}
		}
		else if(line -> ncommands >= 2){//si son 2+ mandatos
				
				
				in=0;
				int pidAux=0;//variable auxiliar
				for(contProc=0; contProc<line->ncommands-1;contProc++){ //por cada mandato que hay menos el último
					pipe(pipa);
					pidAux=(int)nuevo_proc(in,pipa[1],line->commands[contProc].filename,line->commands[contProc].argv); //usamos la funcion auxiliar para hacer el fork
					close(pipa[1]);
					in=pipa[0]; //para la siguiente iteración, entra como parametro en la funcion la parte de lectura de pipa.
				}
				ult=fork(); //para el último mandato, el hijo redirige a salida estándar o especificada y ejecuta el ultimo mandato
				if(ult==0){
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

					}
					else dup2(in,0);					
					execvp(line->commands[contProc].filename,line->commands[contProc].argv);
				}
				else{
					//el padre actua igual que 1comando
					if(line->background==0){
						waitpid(ult,&status,0);
					}
					else{
						InsertarPID(pidAux,line->commands[contProc].filename);
					}
				}
				
			}
			printf("$: ");
			
		}
	return 0;
	}

