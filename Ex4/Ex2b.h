#ifndef EX2B
#define EX2B
#endif
#include "Graph.c"
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>



/*
	struct that contains all the data for each thread 
*/
typedef struct Data
{
	
	int timeOut; // times i trying to connect
	int dvSize;	// the size of the dv arry
	struct DataForCalcu* calculatorData; // all the data to the calculator function
	Vertex* me;
	Vertex* neighbor;
	int* checkArry;
	int* waitSenderArry;
	// int DONE;
	int myDvWasChanged;
	pthread_t thread_num;
	
}Data;
/*
	struct for the data i want to send to the calculator function.
*/

typedef struct DataForCalcu 
{
	char** listRouters;
	struct Router** MyRouters;
	int* myWeightToNeighbors;


}DataForCalcu;



/*
	Function that create threads each thread gets a data struct 
*/
void createThreads(int num, struct Graph* g, Vertex* ver,int times);

/*
	Function that create a socket and return the the fd if the create faild the function return -1;
*/
int createSocket(char* s_addr, int port, char* name, int time);

/*
	Function that gets a struct data and check if need to change dv
*/
void* receiver(void* v);

/*
	Function that gets a struct data and send dv to all the neighbors 
*/
void* sender(void* v);

/*
	Function that get a string ant return the ascii value of that string
*/
int AsciiName(char* name);

/*
	Function that create socket for server the function get ip port and the server name
*/
int createSocketReceiver(char* s_addr, int port, char* name);

/*
	Function that calculate the best dv, if there is no more improves from the neighbors the calculator ends the program
*/
void* calculator(void* calc);

/*
	Function that free all the data in DataForCalcu struct
*/
void freeDataForCalcu(DataForCalcu* d);

/*
	Function that serach a name in arry and return the index 
*/
int findMyIndex(char** names, char* who, int viaSize);

/*
	Function that get an arry, and puts -1 in each cell 
*/
void initilaizeArry(int* arry, int size);

/*
	Function that gets a Router arry and a name of a Router to search in the arry, the function return a pointer 
	to that Router
*/
Router* findRouter(Router** arry, char* name, int arrySize);

/*
	Function that gets a new dv and replace it with the old one,
*/
int* changeDv(int* newDv, int* oldDv ,int sizeOfDv);

/*
	Function that print the Routing table of the Vertex taht given
*/
void printTheEnd(Graph* g,Vertex* ver, int sizeOfDv);

/*
	Function that serch the weight between two vertexes and return it
 */
int getWeight(struct Vertex* me,struct Vertex* ne, int size);