#include "smtp.h"


char* getSuffix(char* ser_addr,int port, int fd)
{
	int i;
	int nbytes = 0; // the number bytes reads from the socket
	int curSize =0; // sum all the nbytes that we read/
	char* suffix;  
	char* endline ="\n";
	char* tempRead= (char*)malloc((BUFFER_SIZE)*sizeof(char)); /// temp char* to read from the socket
	char* readS = (char*)malloc((BUFFER_SIZE)*sizeof(char)); // readS a char* taht gets all th socket answer
	memset(&readS[0],0,BUFFER_SIZE);
	memset(&tempRead[0],0,BUFFER_SIZE);
	while(1) // loop that reads until the end of the line.
	{
		nbytes= read(fd,tempRead,BUFFER_SIZE-1);
		strcat(readS,tempRead);
		strcat(readS,"\0");
		if (strstr(readS,endline)!=NULL)
			break;
		memset(&tempRead[0],0,BUFFER_SIZE);
		curSize+=nbytes+1;
		 if((curSize+nbytes)>BUFFER_SIZE)
		{
			readS =realloc(readS,(curSize+nbytes)*sizeof(char));	
			memset(&readS[curSize+1],0,nbytes-1);
		}
		curSize +=nbytes;
	}
	
	printf("S: %s",readS );
	for(i=strlen(readS); i>0;i--) //loop that gets the suffix from the answer of the server
	{		
		if(readS[i]==' ')
			break;
	}
	if(i==0)
	{
		free(readS);
		readS=NULL;
		return NULL;
	}
	suffix = (char*)malloc((curSize)*sizeof(char));
	memset(&suffix[0],0,curSize);
	strncpy(suffix,readS+i+1,(curSize));
	for(i=0;i<strlen(suffix);i++) // loop that make shure the suffix is ok
	{
		if(!(suffix[i]>='a' && suffix[i]<='z') && !(suffix[i]>='A' && suffix[i]<='Z'))
		{
			suffix[i] = '\0';
			break;
		}
	}
	free(readS);
	readS=NULL;
	free(tempRead);
	tempRead =NULL;
	return suffix;

}
void readFromFile(char* fileName, char* suffix, int fd, char* serverNam)
{
	if(fileName==NULL || suffix==NULL || serverNam==NULL)
	{			
		close(fd);
		return;
	}
	FILE* file;
	char* helo = "HELO "; 
	char* RCPT = "RCPT TO: ";
	char* MAIL = "MAIL FROM: ";
	char* commands=NULL;
	int size = (int)strlen(helo)+(int)strlen(serverNam)+1;
	char* heloServer = (char*)malloc((size+1)*sizeof(char));

	memset(&heloServer[0],0,strlen(heloServer));

	strcat(heloServer,helo);
	strcat(heloServer,serverNam);
	strcat(heloServer,"\n");
	int validNum; // a number to know if the server is ok or not after each command
	
	commands = (char*)malloc((strlen(MAIL)+1)*sizeof(char)); // string for the command that we want to send the server*
	memset(&commands[0],0,strlen(commands));
	strcpy(commands,MAIL);
	// memset(&commands[strlen(MAIL)],0,strlen(commands));
	size_t len = 0;
	char* readFile = NULL; // string that gets each line from the file
	if((file = fopen(fileName,"r")) < 0)
	{
		perror("FILE ERROR");
		exit(1);
	}
	// say helo to the  server
	write(fd,heloServer,strlen(heloServer));
	printf("C: %s",heloServer);
	free(heloServer);
	heloServer=NULL;
	validNum=readFromServer(fd,"\n"); // get the answer from the server
	if (validNum==-1)
		freeAllCloseAll(fd,file,commands,suffix);

	getline(&readFile,&len,file);	//read line from the file
	commands = realloc(commands,strlen(commands)+(int)len);
	memset(&commands[10],0,len);
	strcat(commands,readFile);
	strcat(commands,"\r\n");
	write(fd,commands,strlen(commands)); // write mail from command
	printf("C: %s",commands);
	free(readFile);
	readFile = NULL;
	len =0;
	validNum = readFromServer(fd,"\n"); // read the answer
	if (validNum==-1)
		freeAllCloseAll(fd,file,commands,suffix);
	
	memset(&commands[0],0,strlen(commands));
	getline(&readFile,&len,file); // read line from the file
	if(len >strlen(commands))
	{
		commands = realloc(commands,len);
		memset(&commands[0],0,strlen(commands));
	}
	strcpy(commands,RCPT);
	strcat(commands,readFile);
	strcat(commands,"\r\n");
	write(fd,commands,strlen(commands)); // write rcpt to command
	printf("C: %s",commands);
	free(readFile);
	readFile = NULL;
	len = 0;
	validNum = readFromServer(fd,"\n");
	if(validNum==-1)
		freeAllCloseAll(fd,file,suffix,commands);
	

	memset(&commands[0],0,strlen(commands));
	strcpy(commands,"DATA \r\n"); // write data command
	printf("C: %s",commands);
	write(fd,commands,strlen(commands));
	validNum = readFromServer(fd,"\n");
	if (validNum==-1)
		freeAllCloseAll(fd,file,suffix,commands);

	memset(&commands[0],0,strlen(commands));

	while(1)
	{
		if (getline(&readFile,&len,file)<=0) // read from the file and write all the data to the server
		{ 
			free(readFile);
			readFile=NULL;
			len =0;
			break;
		}

		printf("C: %s",readFile);
		write(fd,readFile,len);
		free(readFile);
		readFile=NULL;
		len =0;
	}

	printf("C: .\n");
	strcpy(commands,"\r\n.\r\n"); // end data command
	write(fd,commands,strlen(commands));
	validNum =readFromServer(fd,suffix);
	if (validNum==-1)
		freeAllCloseAll(fd,file,suffix,commands);
	
	memset(&commands[0],0,strlen(commands));
	strcpy(commands,"quit \n"); // end the connection with quit command
	printf("C: %s",commands);
	write(fd,commands,strlen(commands));
	validNum =readFromServer(fd,suffix);

	freeAllCloseAll(fd,file,suffix,commands);
}



int readFromServer(int fd, char* suffix)
{
	if(suffix==NULL)
		return -1;
	int nbytes=0;
	int curSize; // for the cournt size of the buffer
	int validNum; 
	char* tempRead = (char*)malloc(BUFFER_SIZE*sizeof(char)); 
	char* readFrom = (char*)malloc(BUFFER_SIZE*sizeof(char)); // string for all the answer of the server/
	memset(&tempRead[0],0,BUFFER_SIZE);
	memset(&readFrom[0],0,BUFFER_SIZE);
	curSize =0;
	while(1)
	{
		nbytes= read(fd,tempRead,BUFFER_SIZE-1);
		strcat(readFrom,tempRead);
		strcat(readFrom,"\0");
		if (strstr(readFrom,suffix)!=NULL) /// read until the suffix
		{
			printf("S: %s\n", readFrom);
			validNum = checkCommand(readFrom);
			free(tempRead);
			tempRead = NULL;
			free(readFrom);
			readFrom=NULL;
			if (validNum==-1)
			{
				perror("SERVER ERROR");
				return -1;
			}
			return 0;
		}
		memset(&tempRead[0],0,BUFFER_SIZE);
		curSize+=nbytes+1;
		 if((curSize+nbytes)>BUFFER_SIZE)
		{
			readFrom =realloc(readFrom,(curSize+nbytes)*sizeof(char));	
			memset(&readFrom[curSize+1],0,nbytes-1);
		}
		curSize +=nbytes;
	}
}
int checkCommand(char* line)
{
	int valid = 0;
	char validNum[4];
	memset(&validNum[0],0,4);
	if(line!=NULL)
	{
		memcpy(&validNum[0],line,3);
		strcat(validNum,"\0");
		valid = (int)atoi(validNum);
		if(valid<400)
			return 0;
	}
	return -1;
}

void freeAllCloseAll(int fd,FILE* file, char* suffix,char* command)
{
	if (suffix!=NULL)
	{
		free(suffix);
		suffix = NULL;
	}
	if (command!=NULL)
	{
		free(command);
		command = NULL;
	}
	close(fd);
	fclose(file);
	exit(1);
}


int createSocket(char* s_addr, int port)
{
	int fd;				      /* socket descriptor */
	struct sockaddr_in srv;	      /* used by bind() */
				          

	struct hostent *hp; /*ptr to host info for remote*/ 
	srv.sin_family = AF_INET;
	srv.sin_port = htons(port); 
	hp = gethostbyname(s_addr);

	srv.sin_addr.s_addr = ((struct in_addr*)(hp->h_addr))->s_addr;	

	if((fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		exit(1);
	}


	if(connect(fd,(struct sockaddr *)&srv, sizeof(srv))<0)
	{
		perror("faild");
		exit(1);
	}
	return fd;
}