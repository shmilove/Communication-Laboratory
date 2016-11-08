#include "Ex2b.c"


int main(int argc, char* argv[])
{

	if (argc<3)
	{
		printf("To few argumants\n");
		return -1 ;
	}
	Graph* myGraph = readFile(argv[1]);
	Vertex* ver =NULL;
	int times;

	if(myGraph!=NULL)
	{
		initilaize(myGraph,argv[2]);
		ver = getVertex(argv[2],myGraph->verList,myGraph->vertexSize);

		times =atoi(argv[3]);
		createThreads(ver-> myNeughbersNum , myGraph,ver,times);
		printTheEnd(myGraph,ver, myGraph->vertexSize);
		freeGraph(myGraph,ver-> myNeughbersNum);
	}
	return 0;	
}

