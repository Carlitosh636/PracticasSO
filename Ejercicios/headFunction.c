#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
	int lineas;
	char **array;
	char buf[1024];
	int cuenta =0;
	int i;
	if(argc!=2){
		lineas=10; //estandar si no se pasa un numero indicado
	}
	else{	
		lineas = atoi(argv[1]); //tomamos el numero de lineas a escribir

	}
	array=malloc(lineas*sizeof(char *));
	while((cuenta<lineas) && (fgets(buf, 1024, stdin) != NULL)){
		array[cuenta]=malloc(1024);
		strcpy(array[cuenta],buf);
		cuenta++;
	}
	for(i=0;i<lineas;i++){
		printf("%s",array[i]);
		free(array[i]);
	}
	free(array);
	return 0;
}
