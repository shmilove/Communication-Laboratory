#ifndef EX2
#define Ex2
#endif
#include <stdio.h> // standart lobrary for input outout function
#include <stdlib.h> // library for memory management
#include <string.h>

# define INFINITY 25646;

/* Vertex is a struct that hold vertex in his best nebor vertex and his distance from him
*/
typedef struct Vertex 
{
	char* vertexName;
	int distance;
	struct Vertex *dad;
	
}Vertex;

/*
	struct for edge; each edge has 2 vertexes source and destination and allso the weight between them; 
*/
typedef struct Edge
{
	Vertex* source;
	Vertex* destination;
	int weight;
}Edge;

/*
struct for a graph; each grah hold an arry of the edges and the vertexes the he have;
*/
typedef struct Graph
{
	Vertex** vert;
	Edge** edges;
	int edgeSize;
	int vertexSize;
}Graph;


/*
	function that read from a file;
*/
void readFromFile();

/*
	function that get an vertexes source , destination and the weight and create the edge;
	after the edge created the function return a pointer to him 
*/
Edge* createEdge(Vertex *source,Vertex *destination, int weight);

/*
	function that  gets the size of the vertex arry and the size of the edge arry and create graph;
*/
Graph* createGraph(int vertexSize, int edgeSize);

/*
	funtion that create Vertex by the name that given and initilize the dad to null 
*/
Vertex* createVertex(char* verName);

/*
	function that free the vertex from the memory
*/
void freeVertex(Vertex *ver);

/*
	function that free the graph from the memory
*/
void freeGraph(Graph* gr);

/*
	function that free the edge from the memory
*/
void freeEdge(Edge* edge);

/*
	function that print all the vertexes and there distance from the ver that given 
*/
void printAllVertexesDistance(Graph* myGraph,Vertex* ver);
/*
	Function that cheaks the sortest distance between all the vertexes
*/
void BelmanFord(Vertex* source,Graph *myGraph);

/*
	function that initilize all the vertexes distane to inifinity
*/
int initialize(Graph* myGraph, Vertex* ver);
/*
	function that add vertex to the graph if the vertex is allready exist the function return vertex pointer to the exist one
*/
Vertex* addVertexToGraph(Graph* myGraph, Vertex* ver);

/*
	function that add edge to the graph
*/
void addEdgeToGraph(Graph* myGraph, Edge* edge);
