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



int main(int MainArgc, char *MainArgv[]){
	
	bool run =true;
	char input[] = {};
	char exitCommand[] = "exit\n";

	char *command[1024];

	//variables for getline function
	char buffer[32];
    char *b = buffer;
    size_t bufsize = 32;
    size_t characters;

	
	while (characters != EOF)
	{
    	printf("witsshell>");
    	characters = getline(&b,&bufsize,stdin);

		int comp = strcmp(b,exitCommand);

		if(comp==0){
			
			exit(0);
		}
		//printf("%u",comp);
		char *found;
		int arraySize = 0;
		while( (found = strsep(&b," ")) != NULL ){
			//printf("%s\n",found);
			
			command[arraySize] = found;
			arraySize++;
		}

		for (int i = 0; i < arraySize; i++)
		{
			printf("%s\n",command[i]);
		}
		
		

	}
	

	//return(0);
	exit(0);
}
