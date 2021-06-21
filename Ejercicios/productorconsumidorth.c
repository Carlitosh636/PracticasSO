#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>


pthread_mutex_t mutex;
pthread_cond_t hay_num,consumido;//necesitamos 2 condiciones ya que si el productor no tuviese algo a lo que esperar, se pondría constantemente a producir y no dejaría hacer cosas al consumidor.
char buff[1024];
char *buffCritico;
FILE *fEntrada,*fSalida;
int termino,otro=1;
void *productor(void *args){
	char * file= (char *)args;
	fEntrada=fopen(file,"r");
	char *ptr;
	
	while((ptr=fgets(buff,1024,fEntrada))!=NULL){ 
		pthread_mutex_lock(&mutex);
		while(otro!=1)
			pthread_cond_wait(&consumido,&mutex);
		buffCritico=buff;
		printf("El buff en productor %s\n",buffCritico);
		pthread_cond_signal(&hay_num);
		otro--;
		pthread_mutex_unlock(&mutex);
	}
	
	termino=1;
	fclose(fEntrada);
	pthread_exit(NULL);
	

}
void *consumidor(void *args){
	char * file= (char *)args;
	
	fSalida=fopen(file,"w");
	
	while(termino==0){
		pthread_mutex_lock(&mutex);
		while(buffCritico==NULL)
			pthread_cond_wait(&hay_num,&mutex);
		fputs(buffCritico,fSalida);
		printf("El buff en consumidor %s\n",buffCritico);
		fflush(fSalida);
		buffCritico=NULL;
		otro++;
		pthread_cond_signal(&consumido);
		pthread_mutex_unlock(&mutex);
	}
	
	fclose(fSalida);
	pthread_exit(NULL);
	

}
int main(int argc, char **argv){
	pthread_t tid1,tid2;
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&hay_num,NULL);
	pthread_cond_init(&consumido,NULL);
	pthread_create(&tid1,NULL,productor,(void *) argv[1]);
	pthread_create(&tid2,NULL,consumidor,(void *) argv[2]);
	
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_mutex_destroy(&mutex);
	return 0;
	
	
}
