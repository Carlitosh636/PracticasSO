#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "parser.h"



//Función auxiliar para 2 o más procesos. toma valores para las redirecciones de entrada/salida de los procesos y los mandatos con argumentos que ejecutar
int nuevo_proc(int in,int out,char *mand,char **args,tline * line,int fichero){
	pid_t pid;
	pid=fork();
	if(pid==0){
		if(in!=0){ //el primer mandato no lee del pipe
			if(line->redirect_input != NULL){ //si hay redirección de entrada
						
						fichero = open(line->redirect_input, O_RDONLY);
						
						if (fichero == -1) {
							fprintf(stderr,"%i: Error. Fallo al abrir el fichero de redirección de entrada\n", fichero);
							exit(1);
						} else {
							dup2(fichero,0); //redirijimos la entrada
						}

			}
			else{
				dup2(in,0);			
			}
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
	int id;
	int pid;
	char mandName[1024];
	struct nodo *sig;
};
struct nodo *head=NULL;
static int cont=1;
void MostrarLista(){
	struct nodo *prim =head;
	
	while(prim!=NULL){
		printf("[%d] --> ",prim->id);
		printf("%d ",prim->pid);
		printf("%s ",prim->mandName);	
		if(waitpid(prim->pid,NULL,WNOHANG)==0){			
			printf("%s\n","Running...");	
		}
		else{
			printf("%s\n","Done");
		}
		
		prim=prim->sig;
	}  
}

void InsertarPID(int p,char *name){
	if(head==NULL){
		head=(struct nodo*)malloc(sizeof(struct nodo));
		head->pid=p;
		head->id=cont;
		cont++;
		strcpy(head->mandName,name);
		head->sig=NULL;
		printf("[%d] ",head->id);
		printf("%d ",head->pid);
		printf("%s\n",head->mandName);
	}
	else{
		struct nodo *nuevo=head;
		while(nuevo->sig!=NULL){
			nuevo=nuevo->sig;
		}
		nuevo->sig=(struct nodo*)malloc(sizeof(struct nodo));
		nuevo->sig->pid=p;	
		strcpy(nuevo->sig->mandName,name);		
		nuevo->sig->sig=NULL;
		nuevo->sig->id=cont;
		cont++;
		printf("[%d] ",nuevo->sig->id);
		printf("%d ",nuevo->sig->pid);
		printf("%s\n",nuevo->sig->mandName);
	}
	
}
void sig_handler(int signum){
	printf("\n$: ");
	for(int i=0;i<cont;i++){
		kill(head[i].pid,SIGCHLD);
	}
	
}
void sig_terminate(int signum){
	int pid;
	int status;
	while((pid = waitpid(pid,&status,WNOHANG))>0); //termina el comando en background una vez que ha cumplido su función,y sin interrumpir la shell
}
int main(void) {

	char buf[1024];//buffer entrada
	char bufCD[1024]; //buffer para CD
	tline * line; //linea a parsear
	pid_t pid; //pid del fork
	
	int status;
	int fichero,fichaux,ficherr;//descr de ficheros
	int i,contProc;//contadores
	int in,ult, pipa[2];//pipe y variables para 2+ mandatos
	char *dir;
	i=0;
	printf("$: ");
	signal(SIGCHLD,sig_terminate); //señal para cuando un proceso hijo termina su trabajo. 
	signal(SIGINT,sig_handler); //señales de CTRL C y CTRL Z respectivamente. Si se incluyen
	signal(SIGTSTP,sig_handler);
	while (fgets(buf, 1024, stdin)!=NULL){ //mientras se introduzcan cosas y no se detenga con CTRL+D
		
		line = tokenize(buf);
		if(line == NULL){ //si no se ha introducido
			continue;
		}

		if(strcmp(line-> commands[0].argv[0], "cd")==0){ //si el mandato es cd
			
			if(line->commands[0].argv[1]==NULL){
				dir=getenv("HOME");
				if(dir==NULL){
					fprintf(stderr,"La variable $HOME no se encuentra declarada \n");
					continue;
				}
				printf("%s\n",getcwd(bufCD,1024));
				chdir(getenv("HOME")); // si no tiene argumentos, se pasa al dir estandar $HOME
			}
			if(line->commands[0].argc>2){
				fprintf(stderr,"Uso: %s directorio \n",line-> commands[0].argv[0]); //si se pasan dos directorios (mal uso)
			}
			else{
				dir=line->commands[0].argv[1];
				int checkCD=chdir(dir); //se lleva al dir indicado y si no existe, se avisa.
				if(checkCD==-1){
					printf("El directorio actual no existe: %s \n",dir);
				}
				else printf("%s\n",getcwd(bufCD,1024));
			}
		}
		else if(strcmp(line-> commands[0].argv[0], "jobs")==0){ //si es mandato jobs
			MostrarLista(); //función aux mostrar lista
		}
		else if(strcmp(line-> commands[0].argv[0], "fg")==0){ //si es mandato fg
			int bgPID;
			if(line->commands[0].argv[1]==NULL){ //si no tenemos argumento usamos el ultimo mandato(proceso) en BG
				bgPID=head[cont].pid;
			}
			else{
				bgPID=atoi(line-> commands[0].argv[1]); //tomamos el PID del proceso determinado por parametro
				
			}
			waitpid(bgPID,&status,0);
			
			
		}
		else if(strcmp(line-> commands[0].argv[0], "exit")==0){ //si es mandato exit
			exit(0);
		}
		
		
		else if(line->ncommands == 1){ //Si hay un solo comando
			
				
			pid = fork();
				
			if(pid < 0){  //Error en el fork

				fprintf(stderr, "Fallo del fork() %s/n" , strerror(errno));
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
				
				//espera al hijo si es un proceso para FG. Si no, continúa ejecutando el código e introduce el PID en la lista. Además envía una señal a una función que vigila a que el proceso sea terminado cuando finalice su función (evitar zombies)
				
				
				if(line ->background==0){
					waitpid(pid,&status,0);
				}else{
					InsertarPID(pid,line->commands[0].filename);
					kill(pid,SIGCHLD);
					
				}
				
			}
		}
		else if(line -> ncommands >= 2){//si son 2+ mandatos
				
				
				in=0;
				int pidAux=0;//variable auxiliar
				int hasRedir=0;
				
				for(contProc=0; contProc<line->ncommands-1;contProc++){ //por cada mandato que hay menos el último
					pipe(pipa);
					pidAux=(int)nuevo_proc(in,pipa[1],line->commands[contProc].filename,line->commands[contProc].argv,line,fichero); //usamos la funcion auxiliar para hacer el fork
					close(pipa[1]);
					hasRedir=0;
					in=pipa[0]; //para la siguiente iteración, entra como parametro en la funcion la parte de lectura de pipa.
				}
				ult=fork(); //para el último mandato, el hijo redirige a salida estándar o especificada y ejecuta el ultimo mandato
				if(ult==0){
					
					if (line->redirect_output != NULL) { //si hay redirección de salida
						
						fichero = open(line->redirect_output, O_WRONLY | O_CREAT | O_TRUNC , 0600);

						if (fichero == -1) {
							fprintf(stderr,"%i: Error. Fallo al abrir el fichero de redirección de salida\n", fichero);
							exit(1);
						} else {
							dup2(fichero,1); //redirigimos la salida
						}
					}
					if (line->redirect_error != NULL) { //si hay redirección de error
						
						fichero = open(line->redirect_error, O_WRONLY | O_CREAT | O_TRUNC , 0600);

						if (fichero == -1) {
							fprintf(stderr,"%i: Error. Fallo al abrir el fichero de redirección de salida\n", fichero);
							exit(1);
						} else {
							dup2(fichero,1); //redirigimos la salida de error
						}
					}
					else{
						dup2(in,0);	
					} 				
					execvp(line->commands[contProc].filename,line->commands[contProc].argv);
					fprintf(stderr,"%s: No se encuentra el mandato\n",line->commands[i].filename);
				}
				else{
					//el padre actua igual que 1comando
					if(line->background==0){
						waitpid(ult,&status,0);
					}
					else{
						InsertarPID(pidAux,line->commands[contProc].filename);
						kill(pidAux,SIGCHLD);
					}
				}
				
			}
			printf("$: ");
				
		}
	free(dir);
	free(head); //liberar memoria de arrays
	return 0;
	}

