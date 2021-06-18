#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char **argv){
	char *var;
	var=getenv("PATH");
	char dir[512];
	strcpy(dir,var);
	strcat(dir,"/");
	strcat(dir,argv[1]);
	printf("%s\n",dir);
	if(!access(dir,X_OK)){
		printf("%s\n","El comando existe");
	}
	else{
		printf("%s\n","El comando no se encuentra");
	}
	
}
