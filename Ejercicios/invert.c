#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv){
	char *toCopy= argv[1];
	char *myArray= (char *) malloc((strlen(toCopy)+1)*sizeof(char));

	for(int i=0;i<=strlen(toCopy);i++){
		myArray[i]=toCopy[strlen(toCopy)-i];
		printf("%c",myArray[i]);
	}
	free(myArray);
}
