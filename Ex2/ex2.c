#include "ex2.h"


void readFromFile(char *fileName, char* node)
{

	FILE *ifp;
	char *source, *destination;
	int weight;
	int numberOfVertex;
	int numberOfEdges=0;
	char line[50];
	char* token;
	Graph* myGraph =NULL;	
	Edge* tempEdge=NULL;
	Vertex *from, *to;

	ifp = fopen(fileName, "r");

	if (ifp == NULL)
    {
   		fprintf(stderr, "Can't open input file.\n");
   		exit(1);
   	}
   	fgets(line, 50, ifp);
   	numberOfVertex = atoi(line);

	while ( fgets ( line, 50, ifp) != NULL )
	{
		numberOfEdges++;
	}	
	rewind(ifp);
	myGraph = createGraph(numberOfVertex,numberOfEdges*2);
   	fgets(line, 50, ifp);
	while ( fgets ( line, 50, ifp) != NULL ) /* read a line */
	{
		token = strtok(line , " ");
		source = token;
		token = strtok(NULL , " ");
		destination = token;
		token = (strtok(NULL , " "));
		weight = atoi(token);
		from =createVertex(source);
		to = createVertex(destination);		
		from = addVertexToGraph(myGraph,from);
		to = addVertexToGraph(myGraph,to);
		tempEdge = createEdge(from,to,weight);
		addEdgeToGraph(myGraph,tempEdge);
		
	}
	fclose(ifp);


	from = createVertex(node);
 	BelmanFord(from,myGraph);  		
 	printAllVertexesDistance(myGraph,from);
 	freeGraph(myGraph);
 	freeVertex(from);
	}

Vertex* createVertex(char *verName)
{
	Vertex* myVertex = (Vertex*)malloc(sizeof(Vertex));
	if(myVertex==NULL)
		fprintf(stderr,"create Vertex error\n");
	myVertex -> dad = NULL;
	
	myVertex-> vertexName = (char*)malloc((strlen(verName)+1)*sizeof(char));
	if(myVertex->vertexName==NULL)
		fprintf(stderr,"create Vertex error\n");
	strcpy((myVertex-> vertexName), verName);
	if(verName==NULL)
		fprintf(stderr,"create Vertex error\n");
	return myVertex;
}




Edge* createEdge(Vertex *sour, Vertex *dest, int _weight)
{
	if(sour==NULL || dest==NULL )
		fprintf(stderr,"createEdge error\n");
	Edge* newEdge = (Edge*)malloc(sizeof(Edge));
	if(newEdge==NULL)
		fprintf(stderr,"createEdge error\n");
	newEdge ->source = sour;
	newEdge ->destination =dest;
	newEdge -> weight = _weight;
	return newEdge;
}

Graph* createGraph(int vertexSize, int edgeSize)
{
	int i;
	Graph* myGraph = (Graph*)malloc(sizeof(Graph));
	if(myGraph==NULL)
		fprintf(stderr,"create graph error\n");
	myGraph -> edges = (Edge**)malloc(edgeSize*sizeof(Edge*));
	for(i=0;i<edgeSize;i++)
	{
		myGraph->edges[i] =NULL;
	}
	myGraph -> vert = (Vertex**)(malloc(vertexSize*sizeof(Vertex*)));
	for(i=0;i<vertexSize;i++)
	{
		myGraph-> vert[i] = NULL;
	}
	myGraph -> vertexSize = vertexSize;
	myGraph-> edgeSize = edgeSize;
	return myGraph;
}


void BelmanFord(Vertex* sour,Graph *myGraph)
{
	int i,j;
	i = initialize(myGraph,sour);
	int w = 0;
	if(i==-1)
		fprintf(stderr, "the node is not exists" );
	for(i=0; i<myGraph->vertexSize-1;i++)
	{

		for(j=0;j<myGraph->edgeSize;j++)
		{
			w = myGraph->edges[j]->source->distance + myGraph->edges[j]->weight;			
			if(myGraph->edges[j]->destination->distance > w)
			{
				myGraph->edges[j]->destination->distance = w;

				myGraph->edges[j]->destination->dad = myGraph->edges[j]->source; 
			}	
		}
	}
	for(j=0;j<myGraph->edgeSize;j++)
	{
		if((myGraph->edges[j]->destination->distance)>(myGraph->edges[j]->source->distance) +(myGraph->edges[j]->weight))
			myGraph->edges[j]->destination->distance= -2000;
	}



}

int initialize(Graph* myGraph, Vertex* ver)
{
	int i, status =0;

	for(i=0; i<myGraph->vertexSize;i++)
	{
		if(strcmp(myGraph->vert[i]->vertexName,ver->vertexName)==0)
		{
			(myGraph->vert[i]) ->distance =0;
			myGraph->vert[i] ->dad =myGraph-> vert[i];
			status=1;
		}
		else
			myGraph->vert[i] -> distance = INFINITY; 

	}

	
	if(status==0)
		return -1;
	return 1;
}
Vertex* addVertexToGraph(Graph* myGraph, Vertex* ver)
{
	int i;
	if(ver==NULL|| myGraph==NULL)
		return NULL;
	for(i=0;i< myGraph->vertexSize;i++)
	{
		if(myGraph->vert[i]!=NULL)
			if(strcmp(myGraph->vert[i]->vertexName,ver->vertexName)==0)
			{
				freeVertex(ver);
				return myGraph->vert[i];
			}
	}
	
	for(i=0;i<myGraph->vertexSize;i++)
		{
			if(myGraph->vert[i]==NULL)
			{
				myGraph->vert[i]= ver;
				return myGraph->vert[i];
			}			
		}
		return NULL;
}

void addEdgeToGraph(Graph* myGraph, Edge* edge)
{
	int i;
	Edge* newEdge;
	if(myGraph==NULL || edge ==NULL)
		return;
	for(i=0;i<myGraph->edgeSize;i++)
	{
		if(myGraph->edges[i]!=NULL)
		{
			if((strcmp((myGraph->edges[i])->source->vertexName,(edge->source)->vertexName)==0 )&& 
					(strcmp(myGraph->edges[i]->destination->vertexName,edge->destination->vertexName)==0))
					return;
		 if((strcmp((myGraph->edges[i])->source->vertexName,(edge->destination)->vertexName)==0 )&& 
			(strcmp(myGraph->edges[i]->destination->vertexName,edge->source->vertexName)==0))
			return;
		}
	}
	newEdge = createEdge(edge->destination,edge->source,edge->weight);
	for(i=0; i<myGraph->edgeSize;i++)
	{
		if(myGraph->edges[i]==NULL)
		{
			myGraph->edges[i] = edge;
			break;
		}
	}
	for(i=0; i<myGraph->edgeSize;i++)
	{
		if(myGraph->edges[i]==NULL)
		{
			myGraph->edges[i] = newEdge;
			return;
		}
	}
}


void printAllVertexesDistance(Graph* theGraph, Vertex* ver)
{
	int i;
	Vertex * temp;

	for(i=0 ;i<theGraph->vertexSize; i++)
	{
		if(theGraph->vert[i]->distance <0)
			printf("%s\t%s\t%d\n" , theGraph->vert[i]->vertexName,theGraph->vert[i]->dad ->vertexName, theGraph->vert[i]->distance);
		else
		{
			temp = theGraph->vert[i];
			while(temp->dad!=NULL && strcmp(temp->dad->vertexName, ver->vertexName)!=0)
			{	
				if(temp->dad ==NULL)
					break;
				temp = temp->dad;
			}

				if(temp->dad!=NULL)
					printf("%s\t%s\t%d\n" , theGraph->vert[i]->vertexName,temp ->vertexName, theGraph->vert[i]->distance);		
				else
					printf("%s\tNULL\t%d\n" , theGraph->vert[i]->vertexName, theGraph->vert[i]->distance);

		}
	}
}
 
 void freeVertex(Vertex* ver)
 {
 	ver->dad =NULL;
 	free(ver->vertexName);
 	ver->vertexName=NULL;
 	free(ver);
 	ver=NULL;
 }
void freeEdge(Edge* edge)
{
	edge->source=NULL;
	edge->destination=NULL;
	free(edge);
	edge =NULL;
}

void freeGraph(Graph* myGraph)
{
	int i;
	for(i=0;i<myGraph->edgeSize;i++)
	{
		freeEdge(myGraph->edges[i]);
		myGraph->edges[i]=NULL;
	}
	free(myGraph->edges);
	myGraph->edges=NULL;

	for(i=0;i<myGraph->vertexSize;i++)
	{
		freeVertex(myGraph->vert[i]);
		myGraph->vert[i]=NULL;
	}
	
	free(myGraph->vert);
	myGraph->vert =NULL;
	free(myGraph);
	myGraph=NULL;
}




