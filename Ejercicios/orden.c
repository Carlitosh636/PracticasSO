#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem;
void *escribir1(void *num){
	int i= *(int *) num;
	printf("%s\n","Soy el thread 1");
	sem_post(&sem);
	pthread_exit(NULL);
	

}
void *escribir2(void *num){
	int i= *(int *) num;
	sem_wait(&sem);
	printf("%s\n","Soy el thread 2");
	pthread_exit(NULL);
	

}
int main(int argc, char **argv){
	pthread_t tid1,tid2;
	int n1=1;
	int n2=2;
	pthread_create(&tid1,NULL,escribir1,&n1);
	pthread_create(&tid2,NULL,escribir2,&n2);
	
	sem_init(&sem,0,1);
	
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	sem_destroy(&sem);
	return 0;
	
	
}

