#include "smtp.c"


int main(int argc, char* argv[] )
{
	int port;
	char* suffix;
	if(argc!=3 && argc!=4) 
	{
		perror("to few arguments");
		exit(1);
	}
	if(argc==3)
		port = 25;
	else
		port = atoi(argv[3]);

	int fd = createSocket(argv[2],port);
	suffix = getSuffix(argv[2],port,fd);
	if(suffix!=NULL)
		readFromFile(argv[1],suffix, fd, argv[2]);
	free(suffix);
	suffix =NULL;
	return 0;

}