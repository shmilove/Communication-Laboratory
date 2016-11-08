#include "ex2.c"



int main(int argc, char * argv[])
{
	if(argc<2)
		fprintf(stderr, "err\n");

	readFromFile(argv[1], argv[2]);
	return -1;
}