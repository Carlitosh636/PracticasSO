#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
//NOT FINISHED.NOT FINISHED.NOT FINISHED.NOT FINISHED.NOT FINISHED.
pthread_t threads[20];
int repartidores[20]; //mochila de los repartidores
int paquetes[5][20]; //el primer indice es la sede y el segundo el espacio de paquete, pero en si representa la sede a la que va. Osea los 20 espacios tienen un rango 0-4. El -1 indica que no hay paquete;
pthread_mutex_t mutex;
pthread_cond_t no_lleno;

char buff[1024];

void *funcion(void *num){
	//int id=*(int *)num;
	int sede_sel,sede_dest;
	int lastIDBag=0;
	while(1){
		sede_sel=rand() % 5;
		while((sede_sel=rand() % 5;)==sele_sel)
		
		for(int paq=0;paq<20;paq++){
			if((paquetes[sele_sel][paq]>-1) && (paquetes[sele_sel][paq]==sele_dest)){
				repartidores[lastIDBag]=paquetes[sele_sel][paq]; //pillo todos los paquetes
				lastIDBag++;
				paquetes[sele_sel][paq]=-1;
				
			}
		}
	}
	

}

int main(int argc, char **argv){

	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&no_lleno,NULL);
	for(int j=0;j<5;j++){
		for(int k=0;k<20;k++){
			paquetes[j][k]=rand() % 5; //numero random entre 0 y 4
		}
	}
	for(int i=0;i<20;i++){
		pthread_create(&threads[i],NULL,funcion,NULL);
		
	}
	
	while(1);
	
	
}
