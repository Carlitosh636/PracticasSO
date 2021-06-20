#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

int buff[1000];
pthread_mutex_t mutex;
int cont=0;
void *escribir(void *args){
	while(1){	
		pthread_mutex_lock(&mutex); //sección crítica donde accedemos al buffer para escribir algo
		for(int j=0;j<1000;j++){			
			buff[j]=cont;	
					
		}
		cont++;
		pthread_mutex_unlock(&mutex);
	}
	
	

}
void *comprobar(void *args){
	while(1){
		pthread_mutex_lock(&mutex);
		for(int k=0;k<5;k++){ //sección crítica donde recorremos el buffer para comprobar datos.
					
			if(buff[0]!=buff[k]){
				printf("%s\n","los valores no son iguales ");
			}	
		}
		pthread_mutex_unlock(&mutex);
	}
	
}
int main(int argc, char **argv){
	pthread_t tid1,tid2;
	
	
	pthread_create(&tid1,NULL,escribir,NULL);
	pthread_create(&tid2,NULL,comprobar,NULL);
	pthread_mutex_init(&mutex,NULL);

	
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	
	pthread_mutex_destroy(&mutex);
	
	return 0;
	
	
}
