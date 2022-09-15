#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>


int main(int MainArgc, char *MainArgv[]){
	
	bool run =true;
	char input[] = {};
	char exit[] = "exit\n";

	//variables for getline function
	char buffer[32];
    char *b = buffer;
    size_t bufsize = 32;
    size_t characters;

	
	while (run)
	{
		
		
    	printf("witsshell>");
    	characters = getline(&b,&bufsize,stdin);

		int comp = strcmp(buffer,exit);

		if(comp==0){
			run = false; 
		}

	}
	

	return(0);
}
