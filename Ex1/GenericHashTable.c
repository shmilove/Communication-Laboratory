

#include "GenericHashTable.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>





int intHashFun(int* key, int origSize)
{
	if(*key<0)
		return -1;
	int sum=0;
	sum = *key;
	return sum % origSize;
}

int strHashFun(char* key, int origSize)
{
	int i=0;
	int sum=0;
	
		while(key[i]!='\0')
		{
			sum=sum +key[i];
			i++;
		}	
	return sum % origSize;
}

/* function that add object to the list if there is an empty spot;
	
*/
int addToList(Table* table,int index,Object* obj)
{
	if(table==NULL || obj==NULL)
		return -1;
	int listLength,sum;
	sum=1;
	listLength= table->listLength;
	Object* root; // root is temp object to run on the list
	root = table->arr[index];
	if(table->arr[index]==NULL)
	{
		table ->arr[index] = obj;
		return index;
	}
	while(root->next!= NULL && sum<listLength)
	{
		root = root->next;
		sum++;
	}
	if(root->next==NULL && sum<listLength)
	{
		root->next = obj;
		return index;
	}
	return -1;
}



int add(Table* table, void* data)
{
	if(table==NULL || data==NULL)
		return -1;
	int tempIndex,index,repeat =-1; // repeat cheaks if needed to duble the table agian.
	int i;
	Object *root = NULL;
	root=createObject(data, table->type);
	if (table ->type ==INT_TYPE)
		tempIndex=(intHashFun(((int*)data),table->originalSize));
	else if(table -> type==STR_TYPE)
		tempIndex=(strHashFun(((char*)data),table->originalSize));
	
	while(1)
	{
		index= (table->d)*tempIndex;
		for(i=0;i<table->d;i++)
		{
			repeat = addToList(table,index+i,root);
			if(repeat!=-1)
			{
				return (index+i);		
			}
		
		}
		newTableSize(table);
	}
	return -1;
}

	
	

/**fuction that duble the table */

void newTableSize(Table* table)
{
	int i, newSize;
	newSize = table->corSize*2;
	Object ** newArr;
	newArr = (Object**)(malloc((newSize)*sizeof(Object*)));
	table -> corSize = 2*(table->corSize);
	table -> d= 2*(table->d);
	for(i=0; i<table->corSize;i++)
	{
		newArr[i]= NULL;
	}
	for(i=0;i<((table->corSize/2));i++)
	{
		newArr[i*2] = table ->arr[i];

	} 
	free(table->arr);
	table->arr = NULL;

	(table->arr)= newArr;
}


Table* createTable(int size, int dType, int listLength)
{
	int i;
	Table *table= (Table*)malloc(sizeof(Table)); 
	table -> arr = (Object**)malloc(size*sizeof(Object*)); 
	table -> originalSize = size;	
	table -> type = dType;
	table->listLength =listLength;
	table ->d=1;
	table ->corSize = size;

	for(i=0;i<size;i++)
		table->arr[i]=NULL;

	return table;
}


int removeObj(Table* table, void* data)
{
	if(table==NULL || data ==NULL)
		return -1;
	Object* head;
	Object* tempObj;
	Object* nextObj;
	int i;
	int index, sum;
	head = search(table,data);
	if(head==NULL)
		return-1;
		
	if(table->type==INT_TYPE)
		index = (table->d)*(intHashFun(data,table->originalSize));
	else
		index = (table->d)*(strHashFun(data,table->originalSize));
	for(i=0;i<table->d;i++)
	{
		tempObj = table->arr[index+i];		
		if(head==tempObj)
		{
			table->arr[index+i] = head->next;
			freeObject(head);
			return index+i;
		}	
			nextObj = tempObj->next;
			sum=1;
			while(nextObj!=head &&sum<table->listLength)
			{
				tempObj=tempObj->next;
				nextObj = tempObj->next;
				sum++;
			}
			if(nextObj==head)
			{
				tempObj->next = nextObj->next;
				freeObject(head);
				head=NULL;
				return index+i;
			}	
	}
	return-1;
}






void freeTable(Table* table)
{
	if(table==NULL)
		return;
	int i,size;
	size = table -> corSize;
	for (i=0; i<size; i++)
		if(table->arr[i]!=NULL)
			freeList(table->arr[i]);
	free(table->arr);
	table->arr = NULL;
	free(table);
	table =NULL;
}

Object* createObject(void* data, int type)
{
	if(data==NULL || (type !=STR_TYPE && type!=INT_TYPE))
		return NULL;
	Object *obj= (Object*)malloc(sizeof(Object));
	obj->data = NULL;
	obj->next = NULL;
	if(type == INT_TYPE)
	{
		(obj-> data) = (int*)malloc(sizeof(int));
		*(int*)(obj-> data) = *(int*)data;
	}
	if(type == STR_TYPE)
	{
		(obj -> data) = (char*)malloc((strlen((char*)data)+1)*(sizeof(char)));
		strcpy((char*)obj->data,(char*)data);
	}
	if(obj!=NULL)
		return obj;
	else
		return NULL;
}

Object* search(Table* table, void* data)
{
	if(table==NULL || data ==NULL)
		return NULL;
	
	int index,i,sumList;
	Object* pObj;
	int len = table->d;	
	int ex;
	if(table->type==INT_TYPE)
		index = intHashFun((int*)(data), table->originalSize);
	if(table->type==STR_TYPE)
		index = strHashFun((char*)(data), table->originalSize);

	index = index*(table->d);
	for(i=0;i<len;i++)
	{
		pObj = table->arr[i+index];
		sumList=0;
		while(pObj!=NULL && sumList<table->listLength)
		{
			if(table->type==INT_TYPE)
				ex = isEqual(INT_TYPE,pObj->data,data);
			if(table->type==STR_TYPE)
				ex = isEqual(STR_TYPE,pObj->data,data);
			sumList++;
			if(ex==0)
			{
				printf("%d\t %d\t\n",(index+i) ,sumList );
				return pObj;
			}
			pObj = pObj->next;
		}
	}
	
	return NULL;
}

void freeList(Object* obj)
{
	if(obj==NULL)
		return;
	Object* root =NULL;
	Object* tempObj=NULL;
	if(obj!=NULL && obj->next==NULL)	
	{
		freeObject(obj);
		obj = NULL;
		return;
	}
	else
	{
		root = obj->next;
		freeObject(obj);
		obj=NULL;
		while(root!=NULL)
		{
			tempObj=root->next;
			freeObject(root);
			root = tempObj;
		}
	}
}


void freeObject(Object* obj)
{
		if(obj==NULL)
			return;
		free(obj->data);
		obj->data=NULL;
		free(obj);
		obj=NULL;
}



int isEqual(int type, void* data1, void* data2)
{
	if(data1==NULL || data2==NULL)
		return -1;
	if(type==INT_TYPE)
	{
		if(*(int*)(data1) == *(int*)(data2))
			return 0;
		else
			return -1;
	}
	if(type==STR_TYPE)
		return strcmp((char*)data1,(char*)data2);
	return-1;
}



void printTable(Table* table)
{
	if(table==NULL)
		return;
	int len,i;
	Object *root = NULL;
	len = (table->corSize);
	for (i=0;i<len;i++)
	{
		printf("[%d]\t ",i);
		root = table ->arr[i];
		if(root==NULL)
		{
			printf("\t\n"); 
		}
		while(root!=NULL)
		{	
			if(table->type==INT_TYPE)
				printf("%d\t-->\t",*(int*)root->data);
			else if(table->type==STR_TYPE)
				printf("%s\t-->\t",(char*)root->data);
			// sum++;
			root = (root ->next);
			if(root==NULL)
				printf("\n");
		}						
	}
	root=NULL;
	printf("\n");
}
