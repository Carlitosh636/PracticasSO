#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	int lastNum;
	int firstNum;
	int midNum;
	switch(argc){
		case 1:
			printf("%s","No has puesto ningun parametro, introduce 1 o 2 o 3");
			break;
		case 2:
			lastNum=atoi(argv[1]);
			for(int i=0;i<=lastNum;i++){
				printf("%d",i);
			}
			break;
		case 3:
			firstNum=atoi(argv[1]);
			lastNum=atoi(argv[2]);
			for(int i=firstNum;i<=lastNum;i++){
				printf("%d",i);
			}
			break;
	}
	
}
