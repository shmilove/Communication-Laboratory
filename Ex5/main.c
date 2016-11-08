#include "Echo.c"




int main(int argc, char** argv)
{
	int port = atoi(argv[1]);
	int fd = createSocket(port);
	listenAll(fd);
	
	return -1;
}