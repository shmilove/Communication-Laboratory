#include "Graph.h"



Router* createRouter(Vertex* ver, Graph* g)
{
	Router* myRouter = (Router*)malloc(sizeof(Router));
	myRouter->myName = ver->vertexName;
	myRouter->myDv = (int*)malloc((g->vertexSize)*sizeof(int)); 
	myRouter-> via = (char**)malloc((g->vertexSize)*(sizeof(char*)));
	int i,t,j=0;
	int loop_size = g->vertexSize;
	for(i=0;i<loop_size;i++)
	{
		myRouter->via[i] = NULL;
	}
	
	for (i = 0; i <loop_size; i++)
	{
		myRouter->myDv[i] = INFINITY;
	}

	for(i=0;i<loop_size;i++)
	{ 
		/* this loop checks if the vertex is on an edge if it is that mean he has a neighbor */
		for(t=0;t<loop_size;t++)
		{
			if(j>=g->edgeSize)
				break;
			if(strcmp(g->edgesList[j]->source->vertexName,ver->vertexName)==0 && strcmp(g->edgesList[j]
				->destination->vertexName,g->verList[t]->vertexName)==0 && myRouter->myDv[t]==INFINITY)
					myRouter->myDv[t] = g->edgesList[j]->weight;

			if(strcmp(g->edgesList[j]->destination->vertexName,ver->vertexName)==0 && strcmp(g->edgesList[j]
				->source->vertexName,g->verList[t]->vertexName)==0 && myRouter->myDv[t]==INFINITY)
				myRouter->myDv[t] = g->edgesList[j]->weight;
		}
		j+=2;
	}
	/* this loop initilaize the vertex distance to himself to 0 */
	for(i=0;i<loop_size;i++)
	{
		if(strcmp(ver->vertexName,g->verList[i]->vertexName)==0)
		{
			myRouter->myDv[i]=0;
			break;
		}
	}
	return myRouter;
}
void freeRouter(Router* r, int size)
{
	int i;
	if(r==NULL)
		return;
	r->myName =NULL;
	free(r->myDv);
	r->myDv=NULL;
	for(i=0;i<size;i++)
	{
		r->via[i] =NULL;
	}
	free(r->via);
	r->via = NULL;
	free(r);
	r=NULL;
}

void getAllRouters(Graph* g, Vertex* ver)
{

	Router** myR = (Router**)malloc((ver->myNeughbersNum)*sizeof(Router*));
	Vertex* tempVer =NULL;
	int i;
	for(i=0;i<ver->myNeughbersNum;i++)
	{
		myR[i]=NULL;
	}

	for(i=0;i<ver->myNeughbersNum;i++)
	{
		tempVer = getVertex(ver->neighbors[i], g->verList,g->vertexSize);
		myR[i] = createRouter(tempVer,g);
	}
	ver=NULL;
	g->routers = myR;
}


int isEqual(Vertex* v1, Vertex* v2)
{
	if(strcmp(v1->vertexName,v2->vertexName)==0)
		return 0;
	return-1;

}

void numberOfNeighbers(Vertex* ver, int size)
{
	int sum =0;
	int i;
	for(i=0;i<size;i++)
	{
		if(ver->VertexDV[i]!=INFINITY && ver->VertexDV[i]!=0)
			sum++;
	}
	ver->myNeughbersNum = sum;
}

void createNeighbors(Vertex* ver,int size)
{
	char** myNeighbors = (char**)malloc((ver->myNeughbersNum)*sizeof(char*));
	int i,j;
	for(i=0;i<ver->myNeughbersNum;i++)
		myNeighbors[i]=NULL;
	j=0;
	for(i=0;i<size;i++)
	{
		if(ver->VertexDV[i]!=INFINITY && ver->VertexDV[i]!=0)
		{
			myNeighbors[j] = ver->via[i];
			j++;
		}
	}
	ver->neighbors= myNeighbors;

}





void initilaize(Graph* gr, char* name)
{
	createVia(gr,name);
	Vertex* ver = getVertex(name, gr->verList,gr->vertexSize);
	numberOfNeighbers(ver, gr->vertexSize);
	createNeighbors(ver,gr->vertexSize);
	getAllRouters(gr,ver);
	
}




void createVia(Graph* gr, char* name)
{
	int i;
	char** tempVia =(char**)malloc((gr->vertexSize)*sizeof(char*));
	Vertex* ver =NULL;
	for(i=0; i<gr->vertexSize;i++)
	{
		tempVia[i]=NULL;
	}
	for(i=0;i<gr->vertexSize;i++)
	{
		tempVia[i] = gr->verList[i]->vertexName;
	}
	
	for(i=0;i<gr->vertexSize;i++)
	{
		if(strcmp(name, gr->verList[i]->vertexName)==0)
			ver = gr->verList[i];
	}
	ver->via = tempVia;
	createDV(ver,gr);
}



void createDV(Vertex* ver, Graph* gr)
{
	int i,t,j=0;

	for(i=0;i<gr->vertexSize;i++)
	{
		for(t=0;t<gr->vertexSize;t++)
		{
			if(j>=gr->edgeSize)
				break;
			if(strcmp(gr->edgesList[j]->source->vertexName,ver->vertexName)==0 && strcmp(gr->edgesList[j]
				->destination->vertexName,ver->via[t])==0 && ver->VertexDV[t]==INFINITY)
				ver->VertexDV[t] = gr->edgesList[j]->weight;
			if(strcmp(gr->edgesList[j]->destination->vertexName,ver->vertexName)==0 && strcmp(gr->edgesList[j]
				->source->vertexName,ver->via[t])==0 && ver->VertexDV[t]==INFINITY)
				ver->VertexDV[t] = gr->edgesList[j]->weight;
		}
		j+=2;
	}
	for(i=0;i<gr->vertexSize;i++)
		if(strcmp(ver->vertexName,ver->via[i])==0)
		{
			ver->VertexDV[i]=0;
			break;
		}			
	// for(i=0;i<gr->vertexSize;i++)
	// {
	// 	printf("me: %s via: %s d: %d\n",ver->vertexName,ver->via[i],ver->VertexDV[i]);
	// }

}

void addVertex(Vertex** neighbors,Vertex* ver,int size, int w)
{
	int i;
	for(i=0;i<size;i++)
	{
		if(neighbors[i]!=NULL)
		{
			if(strcmp(neighbors[i]->vertexName,ver->vertexName)==0)
				return;
		}
	}
	for(i=0;i<size;i++)
	{
		if(neighbors[i]==NULL)
		{ 
			neighbors[i] =ver;
			return;
		}
	}

}



Graph* readFile (char* fileName)
{
	FILE* ifp;

	int vertexesNumber =0;
	int EdgesNumber =0;
	Vertex** allVertexes =NULL;
	Edge** allEdges = NULL;
	Graph* gr =NULL;
	ifp = fopen(fileName, "r");

	if (ifp == NULL)
    {
   		fprintf(stderr, "Can't open input file.\n");
   		exit(1);
   	}
   	vertexesNumber = numberOfVertexes(ifp);
   	allVertexes = getAllVertexes(ifp,vertexesNumber);
   	EdgesNumber = numberOfEdges(ifp,vertexesNumber);
   	allEdges = getArryOfEdges(ifp,allVertexes,vertexesNumber,EdgesNumber);
   	gr = createGraph(allVertexes,allEdges,vertexesNumber,EdgesNumber);
   	fclose(ifp);

   	return gr;
   	
}


Graph* createGraph(Vertex** vertexArry,Edge** edgeArry, int vertexesNumber, int EdgesNumber)
{
	Graph* myGraph = (Graph*)malloc(sizeof(Graph));
	myGraph -> verList = vertexArry;
	myGraph -> edgesList = edgeArry;
	myGraph -> edgeSize = EdgesNumber;
	myGraph -> vertexSize = vertexesNumber;
	myGraph ->routers =NULL;
	return myGraph;
}



int numberOfEdges(FILE* file, int numberOfVertexes)
{
	char* readLine = NULL;
	size_t len =0;
	int num =0;
	int i=0;
	while(getline(&readLine,&len,file)>=0)
	{
		num++;
	}
	rewind(file);
	for(i=0;i<numberOfVertexes+1;i++)
	{
		getline(&readLine,&len,file);
	}
	free(readLine);
	return num*2 ;

}


Edge** getArryOfEdges(FILE* file, Vertex** arry, int arrySize, int edgeSize)
{
	char* readLine =NULL;
	char* token =NULL;
	size_t len =0;
	int i=0, weight;	
	Edge* temp1 = NULL;
	Edge* temp2 = NULL;

	Vertex* ver1 =NULL;
	Vertex* ver2 =NULL;
	char* sour =NULL;
	char* dest = NULL;
	Edge** edgeArry = (Edge**)malloc(edgeSize*sizeof(Edge*));
	for(i=0;i<edgeSize;i++)
	{
		edgeArry[i] =NULL;
	}
	i=0;
	while(getline(&readLine,&len,file)>0)
	{

		token = strtok(readLine, " ");
		sour = token;
		ver1 = getVertex(sour,arry,arrySize);
		token = strtok(NULL, " ");
		dest = token;
		ver2 = getVertex(dest,arry,arrySize);
		token = strtok(NULL, " ");
		weight = atoi(token);
		temp1 = createEdge(ver1,ver2,weight);
		edgeArry[i] = temp1;
		temp2 = createEdge(ver2,ver1,weight);


		edgeArry[i+1] = temp2;
		i+=2;
		len =0;
		free(readLine);
		readLine=NULL;
	}
	free(readLine);
	readLine=NULL;
	return edgeArry;
}

Vertex* getVertex(char* name, Vertex** arry, int len)
{
	if(name==NULL || arry ==NULL)
		return NULL;
	int i;
	for(i=0;i<len;i++)
	{
		if(strcmp(name,arry[i]->vertexName)==0)
			return arry[i];
	}
	return NULL;
}



Edge* createEdge(Vertex *sour,Vertex *dest, int _weight)
{
	if(sour==NULL || dest==NULL )
		return NULL;
	Edge* newEdge = (Edge*)malloc(sizeof(Edge));
	if(newEdge==NULL)
	{
		fprintf(stderr, "create Edge error.\n");
		exit(1);
	}
	newEdge ->source = sour;

	newEdge ->destination =dest;

	newEdge -> weight = _weight;
	return newEdge;
}



Vertex** getAllVertexes(FILE* file, int size)
{
	int i;
	char* readLine =NULL;
	char * verName;
	// char* temp;
	char* ip;
	int port;
	char* token;
	size_t len =0;
	Vertex* tempVer = NULL;
	Vertex** ver = (Vertex**)malloc(size*sizeof(Vertex*));
	for(i=0;i<size;i++)
	{
		ver[i]=NULL;
	}
	for (i = 0; i < size; i++)
	{
		getline(&readLine,&len,file);
		token = strtok(readLine, " ");
		verName = token;
		token = strtok(NULL, " ");
		ip = token;
		token = strtok(NULL, " ");
		port = atoi(token);
		len =0;
		tempVer = createVertex(verName, ip,port,size);
		ver[i] = tempVer;
		free(readLine);
		readLine=NULL;
	}
	return ver;
}


Vertex* createVertex(char* verName, char* ip, int port, int size)
{
	Vertex* ver = (Vertex*)malloc(sizeof(Vertex));
	if(ver==NULL)
		fprintf(stderr,"create Vertex error\n");
	ver -> via = NULL;
	ver->neighbors =NULL;
	int i;
	// int DV[size];
	ver ->vertexName = (char*)malloc((strlen(verName)+1)*sizeof(char));
	if((ver->vertexName)==NULL)
		fprintf(stderr,"Vertex Name error\n");
	ver->VertexIP = (char*)malloc((strlen(ip)+1)*sizeof(char));
	if((ver->VertexIP)==NULL)
		fprintf(stderr,"Vertex IP error\n");
	strcpy(ver-> vertexName,verName);
	strcpy(ver-> VertexIP,ip);
	(ver-> VertexPort) = port;	
	ver->VertexDV = (int*)malloc(size*sizeof(int));
	for(i=0;i<size;i++)
		ver->VertexDV[i] = INFINITY;

	return ver;
	
}



void freeVertex(Vertex* ver,int size)
 {
 	int i;
 	if(ver->via!=NULL)
 		for(i=0;i<size;i++)
 			ver->via[i] = NULL;
 	free(ver->via);
 	ver->via =NULL;
 	if(ver->neighbors!=NULL)
 	{
 		for(i=0;i<ver->myNeughbersNum;i++)
 			ver->neighbors[i]=NULL;
 		free(ver->neighbors);
 	}
 	ver->neighbors=NULL;
 
 	for(i=0;i<size;i++)
 		ver->VertexDV[i] = 0;
 	free(ver->VertexDV);
 	ver->VertexDV =NULL;
 	free(ver->vertexName);
 	ver->vertexName=NULL;
 	free(ver->VertexIP);
 	ver->VertexIP =NULL;
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


int numberOfVertexes(FILE* file)
{
	char* readLine = NULL;
	size_t len =0;
	int numberOfVertexes = 0;
	getline(&readLine,&len,file);
	numberOfVertexes = atoi(readLine);
	free(readLine);
	return numberOfVertexes;
}

void freeGraph(Graph* myGraph, int myNeughbersNum)
{
	int i;
	for(i=0;i<myGraph->edgeSize;i++)
	{
		freeEdge(myGraph->edgesList[i]);
		myGraph->edgesList[i]=NULL;
	}
	free(myGraph->edgesList);
	myGraph->edgesList=NULL;

	for(i=0;i<myGraph->vertexSize;i++)
	{
		freeVertex(myGraph->verList[i], myGraph->vertexSize);
		myGraph->verList[i]=NULL;
	}
	free(myGraph->verList);
	myGraph->verList =NULL;
	for(i=0;i<myNeughbersNum;i++)
	{
		freeRouter(myGraph->routers[i], myGraph->vertexSize);
		myGraph->routers[i]=NULL;
	}
	free(myGraph->routers);
	myGraph->routers=NULL;

	free(myGraph);
	myGraph=NULL;
}