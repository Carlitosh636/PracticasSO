#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

int buff_circular[10];
int suma_pares=0;
int suma_impares=0;
pthread_mutex_t mutex;
pthread_cond_t buff_lleno;

void *funcion(void *args){
	pthread_mutex_lock(&mutex);
	while(buff_circular[0]==0)
		pthread_cond_wait(&buff_lleno,&mutex);
	for(int j=0;j<10;j++){
		if(buff_circular[j]<0){
			pthread_exit(NULL);
		}
		if(buff_circular[j] % 2==0){

			suma_pares=suma_pares+buff_circular[j];
		}
		else{

			suma_impares=suma_impares+buff_circular[j];
		}
	}
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

int main(int argc, char **argv){
	int num=atoi(argv[1]);
	int *aleatorios= (int *) malloc(sizeof(int)*num);
	pthread_t thd;
	int i;
	
	pthread_mutex_init(&mutex,NULL);
	pthread_create(&thd,NULL,funcion,NULL);
	pthread_cond_init(&buff_lleno,NULL);
	
	pthread_mutex_lock(&mutex);
	for(i=0;i<num;i++){
		aleatorios[i]=rand() % 999;
		if(i<10){
			buff_circular[i]=aleatorios[i];
		}
	}
	int randID=rand() % 9;
	
	buff_circular[randID]=buff_circular[randID]*-1;
	pthread_cond_signal(&buff_lleno);
	
	pthread_mutex_unlock(&mutex);
	
	pthread_join(thd,NULL);
	
	printf("Los valores del buffer son ");
	for(int j=0;j<10;j++){
		printf("%d ", buff_circular[j]);
	}
	printf("\n");
	printf("Suma de pares es %d\n", suma_pares);
	printf("Suma de impares es %d\n", suma_impares);
	
	
	pthread_mutex_destroy(&mutex);
	return 0;
		
}
