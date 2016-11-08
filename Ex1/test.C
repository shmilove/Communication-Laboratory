#include <stdio.h>
#include <string.h>
#include "GenericHashTable.c"
#include "GenericHashTable.h"

#define N 			1
#define F 			1

#define S 			1
#define S_NON_EXIST	1

#define R 			1
#define R_NON_EXIST	1

#define POPULATE 	1
#define CHECK_INT 	1
#define CHECK_STRING 1
#define FREE_TABLE 	1

int main()
{
	//Input Validation
	Table* t1 = createTable(-1, 1, 1);
	printf("====Input Validation====\n");
	if(t1 != NULL)
	{
		printf("Didn't check size\n");
		if(F)
			freeTable(t1);
	}

	Table* t2 = createTable(1, -1, 1);
	if(t2 != NULL)
	{
		printf("Didn't check type\n");
		if(F)	
			freeTable(t2);
	}
	Table* t3 = createTable(1, 1, -1);
	if(t3 != NULL)
	{
		printf("Didn't check list length\n");
		if(F)
			freeTable(t3);
	}
	
	//Create Table
	if(CHECK_INT)
	{
		Table* t = createTable(4, INT_TYPE, 3);
		printf("====Create Table====\n");
		if(t == NULL)	
			printf("Error on CREATE\n");
		printTable(t);

	
		int x = 2;
		int y = 1;
		int z = 3;
	
		printf("====Add====\n");
	
		printf("New element index: %d\n", add(t, &y));
	
		printf("New element index: %d\n", add(t, &x));
		printf("New element index: %d\n", add(t, &x));
		printf("New element index: %d\n", add(t, &x));

		printTable(t);

		printf("New element index: %d\n", add(t, &x));
		printTable(t);
		printf("New element index: %d\n", add(t, &x));
		printf("New element index: %d\n", add(t, &x));
	
		if(POPULATE)
		{
			printf("New element index: %d\n", add(t, &x));
			printTable(t);
			printf("New element index: %d\n", add(t, &x));
			printf("New element index: %d\n", add(t, &x));
	
			printf("New element index: %d\n", add(t, &x));
			printTable(t);
			printf("New element index: %d\n", add(t, &x));
			printf("New element index: %d\n", add(t, &x));

			printf("New element index: %d\n", add(t, &x));
		}

		printTable(t);
	
		if(S)
		{
			printf("====Search====\n");
			if(N && search(t, NULL) != NULL)
				printf("Error on search\n");
			if(search(t, &x) == NULL)
				printf("Error on search, didn't find %d\n", x);
			if(search(t, &y) == NULL)
				printf("Error on search, didn't find %d\n", y);
			if(S_NON_EXIST && search(t, &z) != NULL)
				printf("Error on search, found non existing element\n");
		}
	
		if(R)
		{
			printf("====Remove====\n");
			if(N && removeObj(t, NULL) != -1)
				printf("Error on remove\n");
			if(removeObj(t, &x) == -1)
				printf("Error on remove\n");
			if(removeObj(t, &y) == -1)
				printf("Error on remove\n");
			if(R_NON_EXIST && removeObj(t, &z) != -1)
				printf("Error on remove\n");
			printTable(t);
		}
	
		if(FREE_TABLE)
			freeTable(t);
	}
	
	
	//====================String====================	
	if(CHECK_STRING)
	{
		printf("=======String Type=======\n");			
		
		Table* tStr = createTable(4, STR_TYPE, 3);
		printf("====Create Table====\n");	
		if(tStr == NULL)	
			printf("Error on CREATE\n");
		printTable(tStr);
	
		
		char* b = "22";
		char* a = "11";
		char* c = "33";
	
		printf("====Add====\n");
	
		printf("New element index: %d\n", add(tStr, a));
	
		printf("New element index: %d\n", add(tStr, b));
		printf("New element index: %d\n", add(tStr, b));
		printf("New element index: %d\n", add(tStr, b));

		printTable(tStr);

		printf("New element index: %d\n", add(tStr, b));
		printTable(tStr);
		printf("New element index: %d\n", add(tStr, b));
		printf("New element index: %d\n", add(tStr, b));
	
		if(POPULATE)
		{
			printf("New element index: %d\n", add(tStr, b));
			printTable(tStr);
			printf("New element index: %d\n", add(tStr, b));
			printf("New element index: %d\n", add(tStr, b));
	
			printf("New element index: %d\n", add(tStr, b));
			printTable(tStr);
			printf("New element index: %d\n", add(tStr, b));
			printf("New element index: %d\n", add(tStr, b));

			printf("New element index: %d\n", add(tStr, b));
		}
			
		printTable(tStr);
	
		if(S)
		{
			printf("====Search====\n");
			if(N && search(tStr, NULL) != NULL)
				printf("Error on search\n");
			if(search(tStr, a) == NULL)
				printf("Error on search\n");
			if(search(tStr, b) == NULL)
				printf("Error on search\n");
			if(S_NON_EXIST && search(tStr, c) != NULL)
				printf("Error on search\n");
		}
	
		if(R)
		{
			printf("====Remove====\n");
			if(N && removeObj(tStr, NULL) != -1)
				printf("Error on remove\n");
			if(removeObj(tStr, a) == -1)
				printf("Error on remove\n");
			if(removeObj(tStr, b) == -1)
				printf("Error on remove\n");
			if(R_NON_EXIST && removeObj(tStr, c) != -1)
				printf("Error on remove\n");
			printTable(tStr);
		}
		
		if(FREE_TABLE)
			freeTable(tStr);
	}

	return 0;
}






