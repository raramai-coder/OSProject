#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<sys/types.h>




char *command[1024];
char *fileName;
int arraySize = 0;
char *pathArray[1024]={"/bin/"};
bool redirect = false;
FILE *fp;


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

//function to execute bin commands
int executeCommand(){
	

	int status;
	char *args[2];
	char *executable = malloc(100);
	
	strcat(executable,pathArray[0]);
	strtok(command[0],"\n");
	strcat(executable,command[0]);
	int pfd;

	if(redirect){
		//printf("must redirecty");
		mode_t mode = S_IRWXU;
		pfd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC ,mode);
		if(fork()==0){
			dup2(pfd,1);
			dup2(pfd,2);
			//close(pfd);
			execv(executable, pathArray);
		}else{
			wait(&status);
			close(pfd);
			return(0);
		}
		//close(pfd);
	}else{
		
		if(fork()==0){
			execv(executable, pathArray);
		}else{
			wait(&status);
			return(0);
		}
	}
	
	
}

int redirection(){

	char redirectCommand[]=">";
	int redirectPos = 0;

	for (int i = 0; i<arraySize;++i)
	{
		int comp = strcmp(command[i],redirectCommand);
		if(comp ==0){
			redirect = true;
			redirectPos = i+1;
		}
	}

	if(redirect){

		int status;
		//printf("must redict : redirection");

		if(fork()==0){
			fileName = command[redirectPos];
			strtok(fileName,"\n");
			strcat(fileName,".txt");
			// //printf("%s\n", fileName);
			// fp = freopen(fileName, "a+", stdout);

			//int file_desc = open("dup.txt", O_WRONLY | O_APPEND);
			
		}else{
			wait(&status);
			//fclose(fp);
			exit(0);
		}
		//fclose(fp);
		//printf("%u\n",fclose(fp));
	}
}

int main(int MainArgc, char *MainArgv[]){
	
	#pragma region variables for getline function
	char buffer[32];
    char *b = buffer;
    size_t bufsize = 32;
    size_t characters;
	#pragma endregion

	
	// freopen ("myfile1.txt","a+",stdout);
  	// printf ("This sentence is redirected to a file.\n");
  	// fclose (stdout);

	while (characters != EOF)
	{
    	printf("witsshell>");
    	characters = getline(&b,&bufsize,stdin);
		separateCommand(b); //separate the input string into an array
		builtInCommand(b);  //checks for built-in commands and runs those
		redirection();
		executeCommand();
		//fclose (stdout);
		// int t;

		//freopen ("myfile1.txt","a+",stdout);
  		//printf ("This sentence is redirected to a file.\n");
		//fflush(stdout);
		//printf ("flushed \n");
		//exit(0);
  		//fclose (stdout);
		//printf("%u\n",fclose(fp));
		// if(fclose(fp)==0){
		// 	printf(fclose(fp));
		// }else{
		// 	perror(" failed to close file");
		// 	exit(-1);
		// }
		//int t = fclose(fp);
		//printf("%d\n", t);
		
		//freopen ("myfile1.txt","w",stdout);
  		//printf ("This sentence is redirected to a file.");
  		//fclose (stdout);
		// if(redirect){
		// 	//close
		// 	printf("trying to close fp");
		// 	fclose(fp);
		// 	printf("closed fp");
		// }
		
	}
	

	exit(0);
}
