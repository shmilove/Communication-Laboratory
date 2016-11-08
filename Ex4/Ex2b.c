#include "Ex2b.h"


pthread_mutex_t my_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t startCalculate = PTHREAD_COND_INITIALIZER; 
pthread_cond_t finishCalculate = PTHREAD_COND_INITIALIZER; 
int myDvWasChanged =-1; // flag to check if the dv changed
int DONE =0; // flag to check if the program needs to end
int firstWrite =0; // flag to check if this is the first read

void* sender(void* v)
{
	
	int i,sfd;
	struct Data* data =  (struct Data*)v;
	int NewDV[data->dvSize+1];
	sfd = createSocket(data->neighbor->VertexIP,data->neighbor->VertexPort,data->me->vertexName, data->timeOut);
	if(sfd<0)
	{
		fprintf(stderr, "%s\n", "Error - Create socket");
		return NULL;		
	}
	while(1)
	{
		pthread_mutex_lock(&my_lock);
		for(i=1;i<data->dvSize +1;i++)
			NewDV[i] = data->me->VertexDV[i-1];
		if(myDvWasChanged==1)
			NewDV[0]=1;
		else
			NewDV[0]=0;
		pthread_mutex_unlock(&my_lock);			
		if(write(sfd,NewDV,(data->dvSize+1)*sizeof(int))<0)
		{
			close(sfd);
			return NULL;
		}

		if(DONE==1)
		{
			close(sfd);
			return NULL;
		}
	
		pthread_mutex_lock(&my_lock);
		if (data->waitSenderArry[data->thread_num]==-1)
		{
			pthread_cond_wait(&finishCalculate,&my_lock);		
		}
		data->waitSenderArry[data->thread_num]=-1;
		pthread_mutex_unlock(&my_lock);			
			
	}	
}
	
	


void* receiver(void* v)
{

	int firstRead = 0;
	int rfd;
	struct Data* data =  (struct Data*)v;
	Router* tempRouter =NULL;
	rfd = createSocketReceiver(data->me->VertexIP,data->me->VertexPort,data->neighbor->vertexName);//neighbor name, my port, my ip
	if(rfd<0)
	{
		fprintf(stderr,  "%s\n", "ERROR - socket failed");
		return NULL;
	}
		
	struct sockaddr_in cli_addr;
	int connectFD =0;
	socklen_t clilen =0;
	int tempDV[(data->dvSize+1)*sizeof(int)];
	clilen = sizeof(cli_addr);
	if((connectFD=accept(rfd,(struct sockaddr *)&cli_addr,&clilen))<0)
	{	
		fprintf(stderr,  "%s\n", "ERROR - accept failed");
		close(rfd);
		return NULL;
	}

	while(1)
	{
		if((read(connectFD,tempDV,(data->dvSize+1)*sizeof(int))<0))
		{
			fprintf(stderr, "%s\n", "ERROR - read failed");
			close(connectFD);
			close(rfd);
			return NULL;
		}
		if (DONE==1)
		{
			close(connectFD);
			close(rfd);
			return NULL;
		}
		pthread_mutex_lock(&my_lock);
		/* finds for who i need to update the dv*/
		tempRouter = findRouter(data->calculatorData->MyRouters,data->neighbor->vertexName,data->me->myNeughbersNum);
		if(firstRead==0)
		{
			tempDV[0]=1;
			firstRead=1;
		}
		/*checking the first cell in the dv arry to know if the calculator improve the dv*/
		if(tempDV[0]==0)			
			data->checkArry[data->thread_num] =0;
		else
		{
			tempRouter->myDv = changeDv(tempDV, tempRouter->myDv,data->dvSize);
			data->checkArry[data->thread_num] =1;
		}
			

		pthread_mutex_unlock(&my_lock);
		pthread_cond_signal(&startCalculate);
	}
}


void* calculator(void* clac)
{
	struct Data* myCalc = (struct Data*)clac;
	Router** AllmyNeighbors;
	int numberOfZeros =0;
	while(1)
	{
		int myWeightToNeighbor =-1;
		int i,j=0, index;
		pthread_mutex_lock(&my_lock);


		for (i = 0; i < myCalc->me->myNeughbersNum; i++)
		{
			if(myCalc->checkArry[i]==-1)
			{
				pthread_cond_wait(&startCalculate, &my_lock);
				i--;
			}

		}
		pthread_mutex_unlock(&my_lock);
		pthread_mutex_lock(&my_lock);
		numberOfZeros = 0;
		/* loop that check if there is no more need to change the dv because we have the best one */
		for (i = 0; i < myCalc->me->myNeughbersNum; i++)
		{
			if(myCalc->checkArry[i]==0)
				numberOfZeros++;				
		}
		if(numberOfZeros==myCalc->me->myNeughbersNum)	
		{
			DONE = 1;
			myDvWasChanged =0;
			pthread_mutex_unlock(&my_lock);
			pthread_cond_broadcast(&finishCalculate);
			return NULL;
		}
		
		AllmyNeighbors = myCalc->calculatorData->MyRouters;
		myDvWasChanged =0;

		for(i=0;i<myCalc->me->myNeughbersNum;i++)
		{
			for(j=0;j<myCalc->dvSize;j++)
			{
				myWeightToNeighbor = myCalc->calculatorData ->myWeightToNeighbors[i];
				if(myCalc->me ->VertexDV[j] > myWeightToNeighbor + AllmyNeighbors[i]->myDv[j])
				{
					myCalc->me ->VertexDV[j] = (myWeightToNeighbor + AllmyNeighbors[i]->myDv[j]);
					index = findMyIndex(myCalc->me->via,myCalc ->calculatorData->listRouters[j], myCalc->dvSize);
					myCalc->me->via[index] = AllmyNeighbors[i]->myName;
					myDvWasChanged =1;
					
				}
			}

		}
		initilaizeArry(myCalc->checkArry,myCalc->me->myNeughbersNum);		
		for (i = 0; i < myCalc->me->myNeughbersNum; i++)
		{
			myCalc->waitSenderArry[i] =0;
		}
		pthread_mutex_unlock(&my_lock);
		pthread_cond_broadcast(&finishCalculate);
		
	}
		
}







void createThreads(int NeughbersNum, struct Graph* g, Vertex* ver,int times)
{
	if(NeughbersNum<0 || g==NULL || ver==NULL)
	{
		return;
	}
	pthread_t *senderThreads = (pthread_t*)malloc(NeughbersNum*sizeof(pthread_t));
	pthread_t *recieverThreads = (pthread_t*)malloc(NeughbersNum*sizeof(pthread_t));
	pthread_t calco;
	Data* threadData = (Data*)malloc(NeughbersNum*sizeof(Data));
	DataForCalcu*  calcData = (DataForCalcu*)malloc(sizeof(DataForCalcu));
	Vertex* Neighbor =NULL;
	int* myWeightToNeighbor = (int*)malloc(NeughbersNum*sizeof(int));
	calcData -> listRouters = ver->via;
	calcData -> MyRouters = g->routers;

	int i;
	int* arryCheck = (int*)malloc(ver->myNeughbersNum*sizeof(int));
	int* waitSenderArry = (int*)malloc(ver->myNeughbersNum*sizeof(int));

	initilaizeArry(arryCheck, ver->myNeughbersNum);
	initilaizeArry(waitSenderArry, ver->myNeughbersNum);

	for(i=0;i<NeughbersNum;i++)
	{
		Neighbor= getVertex(ver->neighbors[i],g->verList,g->vertexSize);
		myWeightToNeighbor[i] = getWeight(ver,Neighbor, g->vertexSize);

	}
	
	calcData ->myWeightToNeighbors =myWeightToNeighbor;
	for(i=0;i<NeughbersNum;i++)
	{
		Neighbor= getVertex(ver->neighbors[i],g->verList,g->vertexSize);
		threadData[i].neighbor = Neighbor;
		threadData[i].me = ver;
		threadData[i].checkArry = arryCheck;
		threadData[i].timeOut =times;
		threadData[i].dvSize = (g->vertexSize);
		threadData[i].calculatorData = calcData;
		threadData[i].waitSenderArry = waitSenderArry;
		threadData[i].thread_num =i;
	}	
	pthread_create(&calco,NULL,calculator,&threadData[0]);
	for(i=0;i<NeughbersNum;i++)
	{
		pthread_create(&senderThreads[i],NULL,sender,&threadData[i]);	
		pthread_create(&recieverThreads[i],NULL,receiver,&threadData[i]);

	}
	for(i=0;i<NeughbersNum;i++)
	{
		pthread_join(recieverThreads[i],NULL);
		pthread_join(senderThreads[i],NULL);
	}
	/* free memory*/
	pthread_join(calco,NULL);
	free(recieverThreads);
	free(senderThreads);
	free(threadData);
	free(arryCheck);
	free(waitSenderArry);
	freeDataForCalcu(calcData);
}


void printTheEnd(Graph* g,Vertex* ver, int sizeOfDv)
{
	int i;
	printf("**************************routing table of %s************************* \n",ver->vertexName );
	for (i = 0; i < sizeOfDv; i++)
	{
		if(ver->VertexDV[i]==INFINITY)
			printf("%s: %s: %s  \n",g->verList[i]->vertexName, "NULL", "oo");
		else 
			printf("%s: %s: %d  \n",g->verList[i]->vertexName, ver->via[i], ver->VertexDV[i]);
	}
}

int* changeDv(int* newDv, int* oldDv ,int sizeOfDv)
{
	int i;
	int* DV = (int*)malloc((sizeOfDv)*sizeof(int));
	for (i = 0; i < sizeOfDv; i++)
	{	DV[i] = newDv[i+1]; }

	free(oldDv);
	return DV;

}
int findMyIndex(char** via, char* who, int viaSize)
{

	int i=0;
	for(i=0;i<viaSize;i++)
	{
		if(strcmp(via[i],who)==0)
			return i;

	}
	return -1;
}


int AsciiName(char* name)
{

	int value =0;
	int i;
	for(i=0;i<strlen(name);i++)
	{
		value+=name[i];
	}
	return value;

}

void initilaizeArry(int* arry, int size)
{
	int i=0;
	for(i=0;i<size;i++)
	{
		arry[i] =-1;
	}
}

Router* findRouter(Router** arry, char* name, int loopSize)
{
	int i;
	for(i=0;i<loopSize;i++)
		if(strcmp(arry[i]->myName,name)==0)
			return arry[i];
	return NULL;
}






int needToStart(int* arry,int arrySize)
{
	int i;

	for (i = 0; i < arrySize; i++)
	{
		if(arry[i]==-1)
			return -1;
	}
	return 1;
}


int createSocketReceiver(char* s_addr1, int port, char* name)
{
	if(s_addr1==NULL||port<0 || name==NULL)
		return -1;


	int fd;
	struct sockaddr_in my_addr;
	struct hostent * ptrHost;
	struct in_addr *ptr; 
	
	fd = socket(AF_INET,SOCK_STREAM,0);
	my_addr.sin_port = htons(port+AsciiName(name));
	if((ptrHost = (struct hostent *) gethostbyname(s_addr1))==NULL)
	{
		perror("gethostbyname");
		return -1;
	}
	ptr=(struct in_addr *)*(ptrHost->h_addr_list);
	my_addr.sin_addr.s_addr = ptr->s_addr;
	my_addr.sin_family = AF_INET;

	if((fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("error create socket");
		return -1;
	}
	int reuse =1;
	if(setsockopt(fd,SOL_SOCKET, SO_REUSEADDR, &reuse ,sizeof(reuse))<0)
	{
		perror(" i hate this !!");
		return -1;
	}
	if(bind(fd, (struct sockaddr*)&my_addr, sizeof(my_addr))<0)
	{
		perror("error on binding");
		close(fd);
		return -1;
	}
	if(listen(fd,3)<0)
	{
		perror("listen error");
		close(fd);
		return -1;
	}
	return fd;
}


int getWeight(Vertex* me, Vertex* ne,int size)
{
	int w =0;
	int i;
	for(i=0;i<size;i++)
	{
		
		if(strcmp(me->via[i],ne->vertexName)==0)
		{
			w=me->VertexDV[i];
			return w;
		}
	}
	return w;
}








int createSocket(char* s_addr, int port, char* name, int times)
{
	if(s_addr==NULL||port<0 || name==NULL)
		return -1;
	

	int asciiValveOfTheName =AsciiName(name);
	int fd;				       //socket descriptor 
	struct sockaddr_in srv;	    //   used by bind() 
				          

	struct hostent *hp; 
	srv.sin_family = AF_INET;
	srv.sin_port = htons(port+asciiValveOfTheName); 
	hp = gethostbyname(s_addr);

	srv.sin_addr.s_addr = ((struct in_addr*)(hp->h_addr))->s_addr;	

	if((fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("create socket error");
		return -1;
	}

	while(times>0)
	{
		if(connect(fd,(struct sockaddr *)&srv, sizeof(srv))<0)
		{
			perror("fail to connect");
			times--;
			sleep(1);

		}
		else
			return fd;
	}
	return -1;
}
void freeDataForCalcu(DataForCalcu* d)
{
	d->listRouters =NULL;
	d->MyRouters =NULL;
	free(d->myWeightToNeighbors);
	free(d);
	d=NULL;


}