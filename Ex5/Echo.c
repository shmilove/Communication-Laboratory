#include "Echo.h"




void listenAll(int fd)
{
	int i,next=0;
	int arrySize =1;
	int tempSize =0;
	// int firstMalloc =0;
	char* writeToALL =NULL;
	struct sockaddr_in cli;
	socklen_t clilen =0;
	int maxfd = fd;
	int j =0;
	int* newfd = (int*)malloc(sizeof(int)); // an arry for all the fd
	// for (i = 0; i < arrySize; i++)
	// {
	// 	newfd[i] =-1;
	// } 		
	newfd[0] =-1;	
	while(1) 
	{
		signal(SIGINT,sig_handler);
		fd_set readfds;
		FD_ZERO(&readfds); FD_SET(fd, &readfds);
		for(i=0;i<arrySize;i++)
		{
			if(newfd[i] !=-1)
				FD_SET(newfd[i],&readfds);
		}
		/* if the flag is true we free all the program and quit*/
		if((select(maxfd+1, &readfds, 0, 0, 0)<0) || finish_the_Server==TRUE)
		{
			here:
			for (i = 0; i < arrySize; i++)
			{
				if(newfd[i]>=0)
					close(newfd[i]);
			}
			free(newfd);
			// newfd =NULL;
			if(writeToALL!=NULL)
			{
				free(writeToALL);
				writeToALL=NULL;
			}
			close(fd);
			return;
			
		}
		if(FD_ISSET(fd, &readfds)) 
		{
			if(arrySize == next-1)
			{
				tempSize = arrySize;
				arrySize = arrySize*2;
				newfd = realloc(newfd,arrySize*sizeof(int));
				if(newfd==NULL)
					goto here;
				for(j=tempSize;j<arrySize;j++)
					newfd[j] =-1;
			}
			newfd[next++] = accept(fd,(struct sockaddr*) &cli, &clilen); 
			arrySize++;
			if(newfd[next-1]>maxfd)
				maxfd = newfd[next-1];
			
		}
		for(j=0;j<arrySize;j++)
		{
			if(newfd[j]==-1)
				continue;
			if(FD_ISSET(newfd[j], &readfds))// && newfd[j]!=fd)
			{
				writeToALL =readFromSocket(newfd[j],"\n");
				if(writeToALL ==NULL)
				{
					newfd[j] =-1;
					FD_CLR(newfd[j],&readfds);
					continue;
				}
				for(i=0;i<arrySize;i++)
				{
					if(newfd[i]==-1)
						continue;
					else if (newfd[i] > 0 && j!=i)
						write(newfd[i],writeToALL,strlen(writeToALL));
				}
				free(writeToALL);
				writeToALL =NULL;
				
			}
		}
	}
}
	






char* readFromSocket(int fd, char* suffix)
{
	if(suffix==NULL)
		return NULL;
	int nbytes=0;
	int firstRead =0;
	int curSize; // for the cournt size of the buffer
	char* tempRead = (char*)malloc((BUFFER_SIZE+1)*sizeof(char)); 
	char* readFrom=NULL;
	memset(&tempRead[0],0,BUFFER_SIZE+1);
	
	curSize =0;
	while(1)
	{
		nbytes= read(fd,tempRead,BUFFER_SIZE+1);
		if(firstRead==0)
		{
			readFrom = (char*)malloc((nbytes+1)*sizeof(char)); 
			memset(&readFrom[0],0,nbytes+1);
			firstRead=1;
		}
		if(nbytes==0)
		{
			free(tempRead);
			tempRead = NULL;
			if(readFrom!=NULL)
			{
				free(readFrom);
				readFrom =NULL;
			}
			return NULL;
		}
		// strcat(readFrom,tempRead);
		memcpy(&readFrom[curSize],tempRead,nbytes);
		// strcat(readFrom,"\0");
		if (strstr(readFrom,suffix)!=NULL) /// read until the suffix
		{
			free(tempRead);
			tempRead = NULL;			
			return readFrom;
		}
		memset(&tempRead[0],0,BUFFER_SIZE+1);
		curSize+=nbytes+1;
		 if((curSize+nbytes)>BUFFER_SIZE+1)
		{
			readFrom =realloc(readFrom,(curSize+nbytes+1)*sizeof(char));	
			memset(&readFrom[curSize+1],0,nbytes);
		}
		curSize +=nbytes;
	}
}



void sig_handler(int signal)
{

	finish_the_Server = TRUE;
}



int createSocket(int port)
{
	if(port<0 )
		return -1;


	int fd;
	struct sockaddr_in my_addr;
	struct hostent * ptrHost;
	struct in_addr *ptr; 
	char* addr = "127.0.0.1"; //local port
	fd = socket(AF_INET,SOCK_STREAM,0);
	my_addr.sin_port = htons(port);
	if((ptrHost = (struct hostent *) gethostbyname(addr))==NULL)
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
		// close(fd);
		return -1;
	}
	if(listen(fd,5)<0)
	{
		perror("listen error");
		return -1;
	}
	return fd;
}


