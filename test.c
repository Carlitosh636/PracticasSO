#include "libreria.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
	if(argc==3){
		int opcion=atoi(argv[1]);
		int parametro=atoi(argv[2]);
		switch(opcion){
			case 1:
				printf("Head seleccionada\n");
				return head(parametro);
				break;
			case 2:
				printf("Tail seleccionada\n");
				return tail(parametro);
				break;
			case 3:
				printf("LongLines seleccionada\n");
				return longlines(parametro);
				break;
		}
	}
	else{
		printf("Funcion llamada incorrectamente");
		return -1;
	}
}
