//
//  main.cpp
//  cOS
//
//  Created by yihong dai on 9/7/14.
//  Copyright (c) 2014 yihong dai. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "Header.h"
char*  readFromStdIn(char * );
int runProcess ( char ** , LinkedList *);
int changeDir(char *);
int addPathFun(char * , LinkedList* );
int displayAllPath(LinkedList* );
int pathOperation (char ** , LinkedList*);
Node* removePathFun( char * , LinkedList*,int * );
int searchPathExist( char * , LinkedList* );
int searchInPath(LinkedList* , char * );


int main(int argc, const char * argv[])
{

	LinkedList * start = (LinkedList *)malloc(sizeof(LinkedList));
		

	while(1)
	{
		//int initialSize = 4;
		char *bufferReadIn = NULL;
		
		
		bufferReadIn = readFromStdIn(bufferReadIn);
		
		
		
		char * delimiter = (char *)" ";
		
		
		
		char *token = strtok(bufferReadIn,delimiter);
		char ** commandLine = (char **)malloc (1024);
		int count = 0 ;
		if(strcmp(token,"exit")==0)
		{
			return EXIT_SUCCESS;
		}
		commandLine[count] = (char *)malloc(strlen(token)+1);
		strcpy(commandLine[count],(const char *)token);
		//printf("the token is%s$",token );
		while(token!= NULL)
		{
			count++;
			
			token = strtok(NULL, delimiter);
			if(token == NULL)
			{
				break;
			}
			//printf("the token is%s$  ",token );

			commandLine[count] = (char *)malloc(strlen(token)+1);
			strcpy(commandLine[count],(const char *)token);
		}
		commandLine[count] = NULL;
		if(strcmp(commandLine[0],"cd")==0)
		{
			int status = changeDir(commandLine[1]);
			if (status<0)
			{
				printf("it goes in ");
				continue;
			}
			
		}
		else if(strcmp(commandLine[0],"path")==0)
		{
			pathOperation (commandLine,  start);
		}
		else
		{
			runProcess ( commandLine, start);
		}
		printf("\n");
		
		
		for( int i = 0; i <= count; i++ )
		{
			free(commandLine[i]);
		}
		free(commandLine);
		free(bufferReadIn);
	}
	return EXIT_SUCCESS;
}

char* readFromStdIn(char * buffer )
{
	// printf(" error messag ", strerror(errno));
	size_t dum;
	int val;
	printf("$ ");
	if((val = (int)getline(&buffer,&dum , stdin) )== -1)
	{
		perror("error: something wrong hhjbjbhjbhjappens\n");
	}
	
	buffer[val-1]='\0';
	//printf("the buffer line is %d ," ,val);
	//printf("the string is %s", buffer);
	
	return buffer;
}

int runProcess ( char ** command, LinkedList* start)
{
	int exist = searchInPath(start,command[0]) ;
	if(exist == 0 )
	{
		perror("error: path for command does not exist");
		return 0;
	}
	pid_t pid;
	int status ;
	pid = fork();
	if(pid == 0 )
	{
		execvp(command[0], command);
		printf(" command is %s" , command[0]);
		printf("errorA: %s" ,strerror(errno));
		exit(EXIT_FAILURE);
	}
	else if(pid>0){
		status =  wait(&pid);
		if(status<0)
		{
			printf("errorB: %s" ,strerror(errno));
		}
	}
	else{
		printf("errorC: %s" ,strerror(errno));
		exit(EXIT_FAILURE);
	}
	return 1;
}

int changeDir( char * path)
{
	
	if(chdir(path) <0)
	{
		printf("errorE: %s" ,strerror(errno));
		return -1;
	}else {
		printf("change dir successfully");
		return 1;
	}
}

int pathOperation (char ** command, LinkedList* start)
{
	if( command[1] ==NULL)
	{
		displayAllPath(start);

	}
	else if(strcmp(command[1],"+" ) == 0)
	{
		int status  = addPathFun(command[2], start);
		if(status == -1)
		{
			printf("the path has already exist \n");
		}
	}
	else if(strcmp(command[1],"-")==0)
	{
		int status = 0 ;
		Node * result = removePathFun(command[2],start, &status);
		if(result == NULL && status==0)
		{
			perror("there is no such path");
		}
		else{
			start->head = result;
			if(result == NULL)
			{
				start->end = NULL;
			}
		}
	}
	else{
		perror("wrong input format");
	}
	return 1;
}

int displayAllPath(LinkedList* start)
{
	Node * begin = start->head;
	printf("\n");
	while(begin != NULL)
	{
		printf(" %s:", begin->path);
		begin = begin->next;
	}
	return 1;
}

int searchInPath(LinkedList* start, char * exec)
{
	Node * begin = start->head;
	int findIt = 0 ;
	while ( begin != NULL)
	{
		int len =(int)strlen(begin->path)+(int)strlen(exec)+2;
		char  filedir[len];
		strcpy(filedir,begin->path);
		strcat(filedir,"/");
		strcat(filedir, exec);
		filedir[len] = '\0';
		FILE * fp;
		
		fp = fopen (filedir, "r");
		if(fp !=NULL)
		{
			fclose(fp);
			findIt = 1;
			return findIt;
		}
		else{
			fclose(fp);
		}
		begin = begin->next;
	}
	return findIt;
}

Node* removePathFun( char * path, LinkedList* start, int * status)
{
	if(start->head == NULL && start->end == NULL)
	{
		perror("the path list is empty   ");
		return NULL;
	}
	else{
		Node * prev = NULL;
		Node * begin = start->head;
		while ( begin != NULL)
		{
			if(strcmp(path,begin->path)==0)
			{
				if(prev == NULL)
				{
					(*status) = 1;
					Node* ptr = begin->next;
					free(begin->path);
					free(begin);
					return ptr;
				}
				else{
					prev->next = begin->next;
					if(begin == start->end)
					{
						start->end = prev;
					}
					free(begin->path);
					free(begin);
					return start->head;
				}
				
			}
			prev = begin;
			begin = begin->next;
		}
		return NULL;
	}
	
}

int searchPathExist( char * path, LinkedList* start)
{
	Node * begin = start->head;
	while(begin != NULL)
	{
		if(strcmp(path,begin->path)==0)
		{
			return 1;
		}
		begin = begin->next;
	}
	return 0 ;
}


int addPathFun(char * path, LinkedList* start)
{
	
	if( searchPathExist(path,start))
	{
		return -1 ;
	}
	if(start->head  == NULL && start->end == NULL)
	{
		start->head = (Node*)malloc(sizeof(Node));
		start->end = start->head;
		start->end->next = NULL;
		
	}
	else{
		Node * temp = (Node*)malloc(sizeof(Node));
		temp->next = NULL;
		start->end->next = temp;
		start->end = temp;
	}
	start->end->path = (char*)malloc(strlen(path)+1);
	strcpy(start->end->path, path);
	(start->end->path)[strlen(path)] ='\0';
	
	return 1;

	
}


