#ifndef GENERIC_HASH_TABLE
#define GENERIC_HASH_TABLE
#endif

#define INT_TYPE 0
#define STR_TYPE 1


typedef struct Object {
	void* data;
	struct Object* next;
	
}Object;

typedef struct Table {
	Object** arr;
	int originalSize ;
	int listLength;
	int d;
	int corSize;
	int type;
}Table;

// function that runs on the list and search for empty spot, if there is an empty spot it enter the object
int addtoList(Table* table,int index,Object* obj);


/*
	function that dubles the curnnet size of the table;
*/
void newTableSize(Table* table);

/**
* The function gets the original size and the type of the data in the table elements.
* it initializes the Table struct members. 
* On success, the function returns a pointer to the new created Table, otherwise, it return NULL.
*/
Table* createTable(int size, int dType, int listLength);
/**
* The function release all the allocated members of struct Table.
*/
void freeTable(Table* table);

/**
* The function adds data to the hashtable (as described in the exe definition)
* On success, the function returns the array index of the added data, otherwise, it return -1.
*/
int add(Table* table, void* data);

/**
* The function removes the Object which its data equals to data, if there are more than one, it removes the first one.
* On success, the function returns the array index of the removed data, otherwise, it returns -1.
* -1 is also returned in the case where there is no such object.
*/
int removeObj(Table* table, void* data);

/**
* The function search for an object that its data is equal to data and returns a pointer to that object.
* If there is no such object or in a case of an error, NULL is returned.
*/
Object* search(Table* table, void* data);

/**
* The function print the table (the format is in the exe definition)
*/
void printTable(Table* table);

/**
* This function creates an object and return the pointer to it or NULL if there is some error.
*/
Object* createObject(void* data, int type);

/**
* This function frees an object,  note the in the case of STR_TYPE, the data
* of the object should also be freed.
*/
void freeObject(Object* obj);

/**
* check the equality of the data of two objects. The implementation is different depending the type of the data.
* the function returns 0 if they are equal or some other value if they are not equal.
*/
int isEqual(int type, void* data1, void* data2);

/**
* returns the hash value of an integer, which is key mod origSize 
*/
int intHashFun(int* key, int origSize);

/**
* returns the hash value of an string, which is m mod origSize, where m is the sum of the ascii value of all the 
* character in key. 
*/
int strHashFun(char* key, int origSize);

/*
the function run on the object list and frees the objects from the memory; 
*/
void freeList(Object* obj);