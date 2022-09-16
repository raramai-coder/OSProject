#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>


#pragma region linked-list-structure

struct node {
   int data;
   int key;
   struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

//display the list
void printList() {
   struct node *ptr = head;
   printf("\n[ ");
	
   //start from the beginning
   while(ptr != NULL) {
      printf("(%d,%d) ",ptr->key,ptr->data);
      ptr = ptr->next;
   }
	
   printf(" ]");
}

//insert link at the first location
void insertFirst(int key, int data) {
   //create a link
   struct node *link = (struct node*) malloc(sizeof(struct node));
	
   link->key = key;
   link->data = data;
	
   //point it to old first node
   link->next = head;
	
   //point first to new first node
   head = link;
}
#pragma endregion 

char *command[1024];
int arraySize = 0;
char *pathArray[1024];

int processPath(){
	char pathCommand[]="path";

	int comp = strcmp(command[0],pathCommand);

	if(comp==0){
			
		//printf("user is setting path");
		for (int i = 1; i < arraySize; i++)
		{
			pathArray[i-1] = command[i];
		}
		
	}

	for (int i = 0; i < arraySize-1; i++)
	{
		printf("%s\n",pathArray[i]);
	}
	//printf("%s\n", command[0]);
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
	//int arraySize = 0;

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

		checkExit(b); //check if user typed exit and execute

		separateCommand(b); //separate the input string into an array

		processPath();

	}
	

	exit(0);
}
