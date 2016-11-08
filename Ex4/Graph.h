#ifndef GRAPH
#define GRAPH
#endif
#include <stdio.h> // standart lobrary for input outout function
#include <stdlib.h> // library for memory management
#include <string.h>

#define INFINITY 22222


/*
	struct for all the vertex data
*/
typedef struct Vertex 
{
	char* vertexName; 
	char** via;
	char** neighbors; 
	int myNeughbersNum;
	char* VertexIP;
	int VertexPort;
	int* VertexDV;
}Vertex;

/*
	struct for the edge data
*/
typedef struct Edge
{
	Vertex* source;
	Vertex* destination;
	int weight;
}Edge;

/*
	strcut for all the graph data 
*/
typedef struct Graph
{
	Vertex** verList;
	Edge** edgesList;
	int edgeSize;
	int vertexSize;
	struct Router** routers;
}Graph;


/*
	struct for each vertex his name and DV
*/
typedef struct Router
{
	char** via;
	char* myName;
	int* myDv;

}Router;


/*

*/
void addVertex(Vertex** neighbors,Vertex* ver,int size,int w);


/*
	Function that create vertex, the function gets vertex name, ip, port and the size of the dv,
	 in the end the function returns vertex.
*/
Vertex* createVertex(char* verName, char* ip, int port, int size);


/*
	Function that get an vertexes source , destination and the weight and create the edge;
	after the edge created the function return a pointer to him 
*/
Edge* createEdge(Vertex *source,Vertex *destination, int weight);

/*
	Function that gets vertex arry and size,  and edge arry and size, then initilaize the graph,
*/
Graph* createGraph(Vertex** vertexArry,Edge** edgeArry, const int vertexesNumber, int EdgesNumber);


/*
	Function that free the graph from the memory
*/
void freeGraph(Graph* gr, int size);


/*
	Function that free the vertex from the memory
*/
void freeVertex(Vertex* ver, int size);


/*
	Function that free the edge from the memory
*/
void freeEdge(Edge* edge);


/*
	Function that gets file name and reads all the data in it
*/
Graph* readFile (char* fileName);


/*
	Function that gets all the text data on the vertexes and create an arry. 
*/
Vertex** getAllVertexes(FILE* file, int numberOfVertexes);


/*
	Function that read in the file how many vertexes there is and returns the number
*/
int numberOfVertexes(FILE* file);


/*
	Function that cheaks how many Edges there is in the file and returns the number
*/
int numberOfEdges(FILE* file, int numberOfVertexes);


/*	
	Function taht read all the text data on the edges and create a arry of them
*/
Edge** getArryOfEdges(FILE* file, Vertex** arry, int arrySize, int edgeArrySize);


/*
	Function that gets a vertex name search it in the arry, and return a pointer to that vertex.
*/
Vertex* getVertex(char* name, Vertex** arry, int len);


/*
	Function that get a vertex name and initiliazle a DV for him and allso a via arry,
	after that saves the data for all the other vertexes in a diffrent struct. 
*/
void initilaize(Graph* gr, char* name);


/*
	Function that create a via for a vertex;
*/
void createVia(Graph* gr, char* name);


/*
	Function that create a DV fo r a Vertex;
*/

int isEqual(Vertex* v1, Vertex* v2);


/*
	Function that cheaks how many Neighbers for the vertex that given and add it to the vertex data
*/
void numberOfNeighbers(Vertex* ver,int size);


/*
	Function that gets a vertex and the number of his neighbors. the Function create a nieghbors arry 
	and add id to the vertex data.
*/
void createNeighbors(Vertex* ver,int numberOfVertexes);


/*
	Function that create a arry DV, this arry saves all the DV for all the vertexes
*/
int** createArryDV(Graph* g, char** NamesArry);


/*
	Function that gets a vertex and the graph and creates a Router that saves the data of that vertex DV and name
*/
Router* createRouter(Vertex* ver, Graph* g);


/*
	Function that free the router from the memory
*/
void freeRouter(Router* r, int size);


/*
	Function that create a Router arry from the Graph data and add it to the graph data
*/
void getAllRouters(Graph* g, Vertex* ver);


/*
	Function the create a DV for a vertex and add it to the vertex data
*/
void createDV(Vertex* ver, Graph* gr);