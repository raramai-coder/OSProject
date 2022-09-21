#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#pragma region global variables
char *command[1024];
char *fileName;
int arraySize = 0;
char *pathArray[1024] = {"/bin/"};
int pathArraySize = 1;
bool redirect = false;
FILE *fp;
int pCommands = 1;
bool parallel = false;
bool mustExit = false;
char *executable[1024];
char *args[1024];
#pragma endregion

#pragma region parallel commands variables
char *pCommand[1024];
int pArraySize = 0;
char *pPathArray[1024];
#pragma endregion

// function to store the path specified by the user
int processPath()
{
	char pathCommand[] = "path";

	int comp = strcmp(command[0], pathCommand);

	if (comp == 0)
	{

		// printf("user is setting path");

		memset(pathArray, 0, sizeof pathArray);
		pathArray[0] = "/bin/";
		// pathArray ={"/bin/"};
		// printf("%s\n",pathArray[0]);

		for (int i = 1; i < arraySize; i++)
		{
			pathArray[i] = command[i];
			++pathArraySize;
		}

		//printf("%u", pathArraySize);

		// for (int i = 0; i < arraySize; i++)
		// {
		// 	printf("%s\n",pathArray[i]);
		// }
	}
}

// function to check whether user has typed exit, built-in function exit
int checkExit(char *b)
{

	char exitCommand[] = "exit";

	int comp = strcmp(b, exitCommand);

	if (comp == 0)
	{
		mustExit = true;
		exit(0);
	}
}

// function to separate input string into parts and store in array
int separateCommand(char *b)
{
	char *found;
	arraySize = 0;
	memset(command, 0, sizeof command);

	while ((found = strsep(&b, " ")) != NULL)
	{
		// printf("%s\n",found);
		strtok(found, "\n");
		command[arraySize] = found;
		arraySize++;
	}

	// for (int i = 0; i < arraySize; i++)
	// {
	// 	printf("%s\n",command[i]);
	// }
}

// function to change directory
int cdCommand()
{
	char cdCommand[] = "cd";

	int comp = strcmp(command[0], cdCommand);

	if (comp == 0)
	{
		// printf("user is changing directory");
		if (arraySize == 2)
		{

			int child = fork();
			if (child == 0)
			{

				strtok(command[1], "\n");
				int ret = chdir(command[1]);
				// int ret = chdir("cdtest");
				if (ret == 0)
				{ // success
					char cwd[256];
					if (getcwd(cwd, sizeof(cwd)) == NULL)
					{
						perror("getcwd() error: ");
					}
					else
					{
						printf("Changed directory to: %s\n", cwd);
					}
				}
				else
				{
					// perror("UNABLE to change directory");
					char error_message[30] = "An error has occurred\n";
					write(STDERR_FILENO, error_message, strlen(error_message));
				}
				// exit(0);
			}
			else
			{
				int s;
				wait(&s);
				exit(0);
			}
		}
		else
		{
			// printf("Error: can't find directory with multiple parts");
			char error_message[30] = "An error has occurred\n";
			write(STDERR_FILENO, error_message, strlen(error_message));
		}
	}

	char cdCommandError[] = "cd\n";

	int compError = strcmp(command[0], cdCommandError);
	if (compError == 0)
	{
		// printf("Error: user is changing directory without path");
		char error_message[30] = "An error has occurred\n";
		write(STDERR_FILENO, error_message, strlen(error_message));
	}
}

// function to check and implement built in commands
int builtInCommand(char *b)
{
	checkExit(b);  // check and run exit built in function
	processPath(); // check and run path built in function
	cdCommand();   // check and run cd built in function
}

bool verifyCommand(){
	bool canExecute = false;
	int result;

	for (int i = 0; i < pathArraySize; i++)
	{
		char *possibleExcutingFile = malloc(1024);
		strcat(possibleExcutingFile,pathArray[i]);
		strcat(possibleExcutingFile,command[0]);
		printf("%s\n", possibleExcutingFile);

		memset(executable,0,sizeof executable);

		result = access (possibleExcutingFile, F_OK);
		if (result == 0)
		{
			printf("can execute the command\n");
			canExecute = true;
			strcat(executable, pathArray[i]);
			//strtok(command[0], "\n");
			strcat(executable, command[0]);
			break;
			
		}
		
		else
		{
			printf("can't execute the command\n");
		}
		
	}
	
	// const char *executingFile = "/bin/looos";


	// result = access (executingFile, F_OK);
	// if (result == 0)
	// {
	// 	printf("can execute the command\n");
	// }else
	// {
	// 	printf("can't execute the command\n");
	// }

	return canExecute;
}

// function to execute bin commands
int executeCommand()
{

	int status;
	// char *args[2];
	// char *executable = malloc(100);
	

	//strcat(executable, pathArray[0]);
	//strtok(command[0], "\n");
	//strcat(executable, command[0]);
	int pfd;

	verifyCommand();
	if(verifyCommand()){
		if (fork() == 0)
		{

			execv(executable, command);
		}
		else
		{
			wait(&status);
			return (0);
		}
	}

	// if (redirect)
	// {
	// 	// printf("must redirecty");

	// 	mode_t mode = S_IRWXU;
	// 	pfd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, mode);
	// 	if (fork() == 0)
	// 	{
	// 		dup2(pfd, 1);
	// 		dup2(pfd, 2);
	// 		// close(pfd);

	// 		execv(executable, command);
	// 	}
	// 	else
	// 	{
	// 		wait(&status);
	// 		close(pfd);
	// 		return (0);
	// 	}
	// 	// close(pfd);
	// }
	// else
	// {

	// 	if (fork() == 0)
	// 	{

	// 		execv(executable, command);
	// 	}
	// 	else
	// 	{
	// 		wait(&status);
	// 		return (0);
	// 	}
	// }
}

// function to handle user redirecting output in terminal
int redirection()
{

	char redirectCommand[] = ">";
	int redirectPos = 0;

	for (int i = 0; i < arraySize; ++i)
	{
		int comp = strcmp(command[i], redirectCommand);
		if (comp == 0)
		{
			redirect = true;
			redirectPos = i + 1;
		}
	}

	if (redirect)
	{

		int status;

		if (fork() == 0)
		{
			fileName = command[redirectPos];
			strtok(fileName, "\n");
			strcat(fileName, ".txt");
		}
		else
		{
			wait(&status);
			exit(0);
		}
	}
}

bool checkParallel()
{
	char parallelCommand[] = "&";
	pCommands = 1;
	// bool parallel = false;
	// int parallelCommands = 1;

	for (int i = 0; i < arraySize; ++i)
	{
		int comp = strcmp(command[i], parallelCommand);
		if (comp == 0)
		{
			if (!parallel)
			{
				parallel = true;
			}

			++pCommands;
		}
	}

	return parallel;
}

int separateParallelCommand(char b[])
{
	char *found;
	pArraySize = 0;
	// memset(pCommand, 0, sizeof pCommand);
	// printf("separating parallel command \n");

	while ((found = strsep(&b, " ")) != NULL)
	{
		// printf("%s\n",found);
		strtok(found, "\n");
		pCommand[pArraySize] = found;
		pArraySize++;
	}

	// pArraySize--;

	// printf("%u\n", pArraySize);

	// for (int i = 0; i < pArraySize; i++)
	// {
	// 	if(i ==pArraySize-1){
	// 		printf("%s\n",pCommand[i]);
	// 	}else{
	// 		printf("%s\t",pCommand[i]);
	// 	}

	// }
}

int pExit(char b[])
{

	// exit(0);
	// mustExit = true;
	strtok(b, "\n");
	char exitCommand[] = "exit";

	int comp = strcmp(b, exitCommand);
	// printf("%s\n",b);

	if (comp == 0)
	{
		// printf("%s\n",b);
		// printf("%i\n", comp);
		// checkExit("exit\n");
		// char *out = "exit\n";
		// checkExit(out);
		// printf("compared to true\n");
		mustExit = true;
		// printf(" in pExit %i\n", mustExit);
		_exit(0);
	}

	// else{
	// 	//printf("%s\n",b);
	// 	//printf("compared to false");
	// 	//printf("%s\n",exitCommand);
	// 	printf("%i\n", comp);
	// }

	// char exitCommandMidline[] = "exit";

	// int compMidline = strcmp(b,exitCommandMidline);
	// //printf("%s\n",b);

	// if(compMidline==0){
	// 	mustExit = true;
	// 	exit(0);
	// }else{
	// 	//printf("%i\n", compMidline);
	// }
}

int pProcessPath(char b[])
{
	char pathCommand[] = "path";
	strtok(b, "\n");
	int comp = strcmp(b, pathCommand);

	if (comp == 0)
	{

		// printf("user is setting path\n");
		//  printf("%s\n", b);

		memset(pathArray, 0, sizeof pathArray);
		pathArray[0] = "/bin/";
		// //pathArray ={"/bin/"};
		// printf("%s\n",pathArray[0]);

		for (int i = 1; i < arraySize; i++)
		{
			pathArray[i] = pCommand[i];
		}

		// printf("%u", arraySize);

		for (int i = 0; i < arraySize; i++)
		{
			printf("%s\n", pathArray[i]);
		}
	}
}

int pBuiltInCommand(char b[])
{
	pExit(b);
}

int pCdCommand()
{
	char cdCommand[] = "cd";

	int comp = strcmp(pCommand[0], cdCommand);

	if (comp == 0)
	{
		// printf("user is changing directory\n");
		if (pArraySize == 3)
		{

			// printf("correct arguments given to cd");
			int child = fork();
			if (child == 0)
			{
				// printf("managed to get child of child process running");
				strtok(pCommand[1], "\n");
				int ret = chdir(pCommand[1]);
				// int ret = chdir("cdtest");
				if (ret == 0)
				{ // success
					// printf("managed to change directory");
					char cwd[256];
					if (getcwd(cwd, sizeof(cwd)) == NULL)
					{
						// perror("getcwd() error: ");
						char error_message[30] = "An error has occurred\n";
						write(STDERR_FILENO, error_message, strlen(error_message));
					}
					else
					{
						printf("Changed directory to: %s\n", cwd);
					}
				}
				else
				{
					// perror("UNABLE to change directory");
					char error_message[30] = "An error has occurred\n";
					write(STDERR_FILENO, error_message, strlen(error_message));
				}
				// exit(0);
			}
			else
			{
				int s;
				wait(&s);
				exit(0);
			}
		}
		else
		{
			// printf("Error: can't find directory with multiple parts");
			char error_message[30] = "An error has occurred\n";
			write(STDERR_FILENO, error_message, strlen(error_message));
		}
	}

	// char cdCommandError[]="cd\n";

	// int compError = strcmp(pCommand[0],cdCommandError);
	// if(compError==0){
	// 	printf("Error: user is changing directory without path");
	// }
}

int pRedirection()
{

	char redirectCommand[] = ">";
	int redirectPos = 0;

	for (int i = 0; i < pArraySize; ++i)
	{
		int comp = strcmp(pCommand[i], redirectCommand);
		if (comp == 0)
		{
			redirect = true;
			redirectPos = i + 1;
		}
	}

	// printf("%u\n",redirectPos);

	if (redirect)
	{

		// printf("must redirect baby");
		int status;

		if (fork() == 0)
		{
			fileName = pCommand[redirectPos];
			// printf("%s\n", fileName);
			strtok(fileName, "\n");
			strcat(fileName, ".txt");
		}
		else
		{
			wait(&status);
			exit(0);
		}
	}
}

int pExecuteCommand()
{
	int status;
	// char *args[2];
	char *executable = malloc(100);

	strcat(executable, pathArray[0]);
	strtok(pCommand[0], "\n");
	strcat(executable, pCommand[0]);
	int pfd;

	// printf("%s\n", executable);

	if (redirect)
	{
		// printf("must redirecty");
		mode_t mode = S_IRWXU;
		// printf("%s\n", fileName);
		pfd = open(fileName, O_WRONLY | O_CREAT | O_APPEND, mode);

		if (fork() == 0)
		{
			dup2(pfd, 1);
			dup2(pfd, 2);
			// close(pfd);
			execv(executable, pCommand);
			close(pfd);
		}
		else
		{
			wait(&status);
			close(pfd);
			return (0);
		}
		close(pfd);
	}
	else
	{
		// printf("must not redirecty");
		if (fork() == 0)
		{
			execv(executable, pCommand);
		}
		else
		{
			wait(&status);
			return (0);
		}
	}
}

int runParallelCommands(char b[])
{
	separateParallelCommand(b);
	pRedirection();
	// for (int i = 0; i < pArraySize; i++)
	// {
	// 	if(i ==pArraySize-1){
	// 		printf("%s\n",pCommand[i]);
	// 	}else{
	// 		printf("%s\t",pCommand[i]);
	// 	}
	// }
	pProcessPath(b);
	pCdCommand();
	// pBuiltInCommand(b);
	// printf(" in pPC %i\n", mustExit);
	//  builtInCommand(b);  //checks for built-in commands and runs those
	//  redirection();
	pExecuteCommand();
}

int parallelCommands()
{

	char commandsArray[1024][1024];
	int commandNumber = 0;
	char *newCommand = malloc(100);

	memset(commandsArray, 0, sizeof commandsArray);

	for (int i = 0; i < arraySize; i++)
	{
		// printf("%s\n", command[i]);
		char block[] = "&";
		int comp = strcmp(command[i], block);

		if (comp == 0)
		{ // if this is the & element
			// commandsArray[commandNumber][commandNumber] = newCommand;
			strcpy(commandsArray[commandNumber], newCommand);
			// printf("%s\n", newCommand);
			memset(newCommand, 0, sizeof newCommand);
			++commandNumber;
			// printf("command number %u\n", commandNumber);
			continue;
		}
		else
		{
			// strcat(newCommand," ");
			strcat(newCommand, command[i]);
			strcat(newCommand, " ");
		}

		if (i == arraySize - 1)
		{
			// printf("last command %s\n", newCommand);
			// printf("command number %u\n", commandNumber);
			++commandNumber;
			// printf("command number %u\n", commandNumber);
			strcpy(commandsArray[commandNumber], newCommand);
			// printf("%s\n",commandsArray[commandNumber+1]);
		}
	}

	// for (int i = 0; i < commandNumber+1; i++)
	// {
	// 	printf("%s\n",commandsArray[i]);
	// }

	if (parallel)
	{
		// printf("paralley command");
		// printf("%u\n", parallelCommands);
		int status;

		pid_t pid;

		for (int i = 0; i < commandNumber + 1; i++)
		{
			if (i != commandNumber - 1)
			{
				pid = fork();

				if (pid == 0)
				{
					printf("child process number : %u\n", i);
					printf("%s\n", commandsArray[i]);
					runParallelCommands(commandsArray[i]);
					// printf("%s\n",commandsArray[i]);
					//  for (int i = 0; i < commandNumber+1; i++)
					//  {
					//  	printf("%s\n",commandsArray[i]);
					//  }
					// printf(" in pForkedChild %i\n", mustExit);
					// pBuiltInCommand(commandsArray[i+1]);
					// printf("%s\n",commandsArray[i+1]);
					// separateParallelCommand(commandsArray[i+1]);
					//  builtInCommand(b);  //checks for built-in commands and runs those
					//  redirection();
					//  executeCommand();
					exit(0);
				}
				// printf(" after each childe %i\n", mustExit);

				separateParallelCommand(commandsArray[i]);
				pBuiltInCommand(commandsArray[i]);
			}
		}

		while ((pid = waitpid(-1, &status, 0)) != -1)
		{
			printf("Process %d terminated\n", pid);
		}
		// printf(" in pMother %i\n", mustExit);
		return (0);

		// pid_t pid = fork();
		// //pBuiltInCommand(commandsArray[0]); //compares to false
		// if(pid == 0){
		// 	printf("child process number : 1 \n");
		// 	runParallelCommands(commandsArray[0]);
		// 	//pBuiltInCommand(commandsArray[0]); // compars to true when run in the child process
		// 	//pBuiltInCommand(commandsArray[0]);
		// 	//printf("%s\n",commandsArray[0]);
		// 	//separateParallelCommand(commandsArray[0]);
		// 	// builtInCommand(b);  //checks for built-in commands and runs those
		// 	// redirection();
		// 	// executeCommand();
		// 	exit(0);
		// 	//return(0);
		// }else{
		// 	// for (int i = 0; i < commandNumber; i++)
		// 	// {
		// 	// 	pid = fork();
		// 	// 	if(pid==0){
		// 	// 		printf("child process number : %u\n", i+2);
		// 	// 		runParallelCommands(commandsArray[i+1]);
		// 	// 		//printf(" in pForkedChild %i\n", mustExit);
		// 	// 		//pBuiltInCommand(commandsArray[i+1]);
		// 	// 		//printf("%s\n",commandsArray[i+1]);
		// 	// 		//separateParallelCommand(commandsArray[i+1]);
		// 	// 		// builtInCommand(b);  //checks for built-in commands and runs those
		// 	// 		// redirection();
		// 	// 		// executeCommand();
		// 	// 		exit(0);
		// 	// 	}
		// 	// 	//printf(" after each childe %i\n", mustExit);

		// 	// 	pBuiltInCommand(commandsArray[i+1]);

		// 	// }
		// 	//wait(&status);
		// 	//exit(0);
		// 	//return(0);

		// 	//pBuiltInCommand(commandsArray[0]); //compares to false

		// 	//printf("Waiting all child.\n");
		// 	while ((pid=waitpid(-1,&status,0))!=-1) {
		// 	printf("Process %d terminated\n",pid);
		// 	}
		// 	//printf(" in pMother %i\n", mustExit);
		// 	return(0);

		// }
	}
}

int main(int MainArgc, char *MainArgv[])
{

#pragma region variables for getline function
	char buffer[1024];
	char *b = buffer;
	size_t bufsize = 1024;
	size_t characters;
#pragma endregion

	while (characters != EOF)
	{
		#pragma region reset variables
		redirect = false;
		parallel = false;
		#pragma endregion

		printf("witsshell>");
		characters = getline(&b, &bufsize, stdin);
		separateCommand(b); // separate the input string into an array
		if (checkParallel())
		{
			parallelCommands(b);
			// printf("we're here");
			//printf("%i\n", mustExit);
		}
		else
		{
			builtInCommand(b); // checks for built-in commands and runs those
			redirection();
			executeCommand();
		}
		// mustExit = true;

		if (mustExit)
		{
			// printf("must exit");
			// exit(0);
			break;
		}
		// parallelCommands();
	}

	exit(0);
}
