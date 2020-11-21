#include "libreria.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int head(int N){
	int lineas;
	char **array;
	char buf[1024];
	int cuenta =0;
	int i;
	if(N <=0){
		lineas=10; //estandar si no se pasa un numero indicado
	}
	else if (N > 0){	
		lineas = N; //tomamos el numero de lineas a escribir

	}
	array=malloc(lineas*sizeof(char *));
	while((cuenta<lineas) && (fgets(buf, 1024, stdin) != NULL)){
		array[cuenta]=malloc(1024);
		strcpy(array[cuenta],buf);
		printf("%s",array[i]);
		cuenta++;
	}
	for(i=0;i<lineas;i++){
		//printf("%s",array[i]);
		free(array[i]);
	}
	free(array);
	return 0;
}

int tail(int N){
	int lineas;
	char buf[1024];
	char **dump;
	int cuenta =0;
	if(N <=0){
		lineas=10; //estandar si no se pasa el numero exacto
	}
	else if (N > 0){	
		lineas = N; //tomamos el numero de lineas a escribir
	}
	dump=malloc(lineas*sizeof(char *)); //se le da al array memoria dinámica igual a Lineas
	while(fgets(buf,1024,stdin)!=NULL){
		if(cuenta<lineas){
			if(dump[cuenta]==NULL){
				dump[cuenta]=malloc(1024);
			}
			strcpy(dump[cuenta],buf);
		}
		else{
			cuenta=0;
			strcpy(dump[cuenta],buf);
		}
		 //En el array se almacenan sólo las últimas líneas leídas. Las más "antiguas" se descartan. Si se sobrepasa el tamaño del array, vuelve al inicio y sustituye la más antigua.
		cuenta++;
	}
	for(int i = 0; i < lineas; i++){
		printf("%s\n",dump[i]);
		free(dump[i]);
		//print de las líneas y liberación de memoria de cada elemento
	}
	free(dump);
	//liberación de memoria del array principal
	return 0;
}

int compare(const void *a, const void *b){
/*función de comparación para el qsort. Ésta debe retornar un valor menor que 0 si a es menor que b y debe colocarse, 0 si son iguales y mayor a 0 si a es mayor a b. 
Sin embargo, eso causa que a se coloque antes que B, ya que qsort por defecto ordena de menor a mayor. 
Como queremos lo contrario, se hace 1-valor que da el opuesto.*/
	const char **v1=(const char **)a;
	const char **v2=(const char **)b;
	int valor=(strlen(*v1)-1)-(strlen(*v2)-1);
	return (1-valor);
}

int longlines(int N){
	char **listaLineas;
	char buf[1024];
	int maxLineas;
	int i,j;
	if(N <=0){
		maxLineas=10; //estandar si no se pasa un numero indicado
	}
	else if (N > 0){	
		maxLineas = N; //tomamos el numero de lineas a escribir

	}
	i=0;
	listaLineas=malloc(maxLineas*sizeof(char *)); //se pone el tamaño del array
	while(fgets(buf,1024,stdin)!=NULL){
		if(i<maxLineas){
			listaLineas[i]=malloc(1024);
			strcpy(listaLineas[i],buf);	
		}
		i++;
		//colocamos las N primeras líneas, que son las que nos interesan realmente.
	}
	qsort(listaLineas,maxLineas,sizeof(char *),compare);
	//llamamos a la función de c qsort (seccion 3 del man) para ordenar la lista de mayor a menor con nuestra propia funcion compare
	for(j=0;j<maxLineas;j++){
		printf("%s",listaLineas[j]);
		free(listaLineas[j]);
		//imprimimos las N mayores lineas y liberamos memoria
	}
	free(listaLineas);
	return 0;
}
