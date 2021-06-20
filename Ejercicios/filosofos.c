#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
char estados[5];
pthread_t filosofos[5];
int palillos[5];
pthread_mutex_t mutex;
pthread_cond_t espera[5]; 

void *funcion(void *args){
	int id=*(int *)args;
	while(1){
		printf("Filosofo %d pensando \n",id); //pensamos
		estados[id]=0;
		sleep((rand()%2)+1);
		
		pthread_mutex_lock(&mutex); //comemos
		//si han cogido aunque sea un palillo, me espero. Recordar que esta comprobación se debe hacer cuando tenemos el mutex en control.
		while((palillos[id]) || (palillos[(id+1)%5]))
			pthread_cond_wait(&espera[id],&mutex);

		palillos[id]=1;
		palillos[(id+1)%5]=1;
		pthread_mutex_unlock(&mutex);		
		printf("Filosofo %d comiendo \n",id);
		estados[id]=1;
		sleep((rand()%2)+1);
		
		pthread_mutex_lock(&mutex); //dejamos todo en su sitio
		palillos[id]=0;
		palillos[(id+1)%5]=0;
		pthread_cond_signal(&espera[id]);
		pthread_cond_signal(&espera[(id+1)%5]); //aviso al del lado que ya están libres los palillos
		pthread_mutex_unlock(&mutex);
		printf("Filosofo %d termino de comer \n",id);
		estados[id]=2;
	}
	
	
	

}
int main(int argc, char **argv){
	pthread_mutex_init(&mutex,NULL);
	for(int i=0;i<5;i++){
		palillos[i]=0;
		
		pthread_cond_init(&espera[i],NULL);
	}
	int uno=1,dos=2,tres=3,cuatro=4,cero=0;
	pthread_create(&filosofos[0],NULL,funcion,(void *)&cero);
	pthread_create(&filosofos[1],NULL,funcion,(void *)&uno);
	pthread_create(&filosofos[2],NULL,funcion,(void *)&dos);
	pthread_create(&filosofos[3],NULL,funcion,(void *)&tres);
	pthread_create(&filosofos[4],NULL,funcion,(void *)&cuatro);
	
	
	while(1);
		
}
