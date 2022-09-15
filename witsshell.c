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
	char exitCommand[] = "exit\n";

	//variables for getline function
	char buffer[32];
    char *b = buffer;
    size_t bufsize = 32;
    size_t characters;

	
	while (characters != EOF)
	{
    	printf("witsshell>");
    	characters = getline(&b,&bufsize,stdin);

		int comp = strcmp(buffer,exitCommand);

		if(comp==0){
			
			//run = false; 
			exit(0);
		}
		printf("%u",comp);
		char *found;

		// while( (found = strsep(&b," ")) != NULL ){
		// 	printf("%s\n",found);
		// }
        

	}
	

	//return(0);
	exit(0);
}
