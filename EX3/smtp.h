#ifndef smtp
#define smtp
#endif

#include <stdio.h> // standart lobrary for input outout function
#include <stdlib.h> // library for memory management
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#define BUFFER_SIZE 2



/*
	Function that read data from the server until the suffix, if the function fails it return -1
	in success 0
*/
int readFromServer(int fd, char* suffix);
/*
	Function that create a socket connection to a server.   
*/
int  createSocket(char* ser_addr, int port);
/*
	Function that read mail from a file and sending it to the server,
	when the function ends the reading its closing the connection and the file.
	if the server send an error the function closing the connection.
	*/
void readFromFile(char* fileName, char* suffix, int fd, char* serverName);
/*
	Function that free all memmory and closing server connection and file  
*/
void freeAllCloseAll(int fd,FILE* file, char* suffix,char* command);
/*
	Function that reads the first line from the server and returns the suffix
*/
char * getSuffix(char* ser_addr,int port, int fd);
/*
	Function that check if some error have been occur in the server answer,
	it return -1 in error and 0 in success 
*/
int checkCommand(char* line);
