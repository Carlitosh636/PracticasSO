#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char **argv){
	char *newDir;
	char buff[512];
	if(argc<=1){
		newDir=getenv("HOME");
	}
	else{
		newDir=argv[1];

	}
	printf("%s\n",getcwd(buff,512));
		if(chdir(newDir)!=0){
			printf("%s\n","No es un directorio");
		}
	printf("%s\n",getcwd(buff,512));
	return 0;
	
	
}
