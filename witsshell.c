#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>


char *command[1024];
int arraySize = 0;
char *pathArray[1024]={"/bin/"};


//function to store the path specified by the user
int processPath(){
	char pathCommand[]="path";

	int comp = strcmp(command[0],pathCommand);

	if(comp==0){
			
		//printf("user is setting path");

		memset(pathArray, 0, sizeof pathArray);
		pathArray[0] ="/bin/";
		//pathArray ={"/bin/"};
		//printf("%s\n",pathArray[0]);

		for (int i = 1; i < arraySize; i++)
		{
			pathArray[i] = command[i];
		}

		//printf("%u", arraySize);

		// for (int i = 0; i < arraySize; i++)
		// {
		// 	printf("%s\n",pathArray[i]);
		// }
		
	}

}

// function to check whether user has typed exit, built-in function exit
int checkExit(char *b){

	char exitCommand[] = "exit\n";

	int comp = strcmp(b,exitCommand);

	if(comp==0){
			
		exit(0);
	}
}

//function to separate input string into parts and store in array
int separateCommand(char *b){
	char *found;
	arraySize = 0;
	memset(command, 0, sizeof command);

	while( (found = strsep(&b," ")) != NULL ){
		//printf("%s\n",found);
			
		command[arraySize] = found;
		arraySize++;
	}

	// for (int i = 0; i < arraySize; i++)
	// {
	// 	printf("%s\n",command[i]);
	// }
}

//function to change directory
int cdCommand(){
	char cdCommand[]="cd";

	int comp = strcmp(command[0],cdCommand);

	if(comp==0){
		//printf("user is changing directory");
		if(arraySize==2){
			
			int child = fork();
			if(child==0){

        		strtok(command[1],"\n");
				int ret = chdir(command[1]);
				//int ret = chdir("cdtest");
        		if (ret == 0) { // success
					char cwd[256];
					if (getcwd(cwd, sizeof(cwd)) == NULL){
						perror("getcwd() error: ");
					}	
    				else{
						printf("Changed directory to: %s\n", cwd);
					}
      					
        		} else {
            		perror("UNABLE to change directory");
        		}
				//exit(0);
				
			}else{
				int s;
				wait(&s);
				exit(0);
			}
			
		}else{
			printf("Error: can't find directory with multiple parts");
		}
	}

	char cdCommandError[]="cd\n";

	int compError = strcmp(command[0],cdCommandError);
	if(compError==0){
		printf("Error: user is changing directory without path");
	}
}

//function to check and implement built in commands
int builtInCommand(char *b){
	checkExit(b);  //check and run exit built in function
	processPath(); // check and run path built in function
	cdCommand(); //check and run cd built in function
}

int executeCommand(){
	//printf("excuting command\n");

	// char *binaryPath = "/bin/ls";
  	// char *args[] = {binaryPath, "-lh", "/home", NULL};
 
  	// execv(binaryPath, args);
 
  	//return 0;

	// pid_t pid;
    // char *const parmList[] = {"/bin/ls", "-l", "/u/userid/dirname", NULL};

    // if ((pid = fork()) == -1)
    //     perror("fork error");
    // else if (pid == 0) {
    //     execv("/bin/ls", parmList);
    //     printf("Return not expected. Must be an execv error.n");
    // }

	int status;
	char *args[2];
	char *executable = malloc(100);
	strcat(executable,pathArray[0]);
	strtok(command[0],"\n");
	strcat(executable,command[0]);
	
	if(fork()==0){
		execv(executable, pathArray);
	}else{
		wait(&status);
		//exit(0);
		return(0);
	}


}

int main(int MainArgc, char *MainArgv[]){
	
	#pragma region variables for getline function
	char buffer[32];
    char *b = buffer;
    size_t bufsize = 32;
    size_t characters;
	#pragma endregion

	
	while (characters != EOF)
	{
    	printf("witsshell>");
    	characters = getline(&b,&bufsize,stdin);
		separateCommand(b); //separate the input string into an array
		builtInCommand(b);  //checks for built-in commands and runs those
		executeCommand();
	}
	

	exit(0);
}
