#ifndef Echo_Server
#define Echo_Server
#endif
#include <stdio.h> // standart lobrary for input outout function
#include <stdlib.h> // library for memory management
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

#define BUFFER_SIZE 30 // buffer for  the reading 
#define TRUE 1
#define FALSE 0

/* a global int to know when to close and free all the memory*/
int finish_the_Server = FALSE;


/* fuunction that create a server socket and return the socket fd*/
int createSocket(int port);


/* function that gets the socket fd of the server and open echo server to all */
void listenAll(int sfd);


/*function that get a socket fd and read all the data in it. in the end the function return all the data in string */
char* readFromSocket(int fd, char* suffix);


/*
 Function that get a signal and the turn on a flag to close and free all the memory and sockets in the program	
*/
void sig_handler(int signal);