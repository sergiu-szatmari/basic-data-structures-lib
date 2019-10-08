#ifndef _CC_HASHTABLE_H_
#define _CC_HASHTABLE_H_

#include <stdlib.h>
#include <stdio.h>

#include "ccreturn.h"

#define HT_SIZE 26

typedef struct
{
	char** Keys;
	int* Values;
	int Size;
	int Capacity;
} HASH_LIST;

typedef struct _CC_HASH_TABLE 
{ 
	int Size;
	HASH_LIST** Lists;
} CC_HASH_TABLE;

// Prints the entire HashTable
int HtPrint(CC_HASH_TABLE* HashTable);

// Creates the HashTable
int HtCreate(CC_HASH_TABLE** HashTable); 

// Destroys the HashTable, freeing every
// key allocated in every list
int HtDestroy(CC_HASH_TABLE** HashTable); 
 
// Sets the value for a certain key;
// It adds the entry <Key, Value> if the
// key doesn't exist
int HtSetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int Value); 

// Gets the value for a certain key. If the 
// key doesn't exist, Value will be INT_MIN;
int HtGetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int* Value); 

// It removes an entry <Key, Value> with a 
// certain key. If the key doesn't exist, 
// the function will return -2 (INCORRECT_PARAMETER)
int HtRemoveKey(CC_HASH_TABLE* HashTable, char* Key); 

// It checks if a key exists in the Hashtable;
// It returns 0 is the key exists, and > 0 else.
int HtHasKey(CC_HASH_TABLE* HashTable, char* Key); 

// It gets the Nth key in the Hashtable. The index
// must be greater than 1 (1 <= Index <= ...). The
// function allocates memory for the key it returns
// in (*Key), so (*Key) must be NULL when the function
// is called. If the Index is greater than the total number
// of elements, (*Key) is going to be NULL;
int HtGetNthKey(CC_HASH_TABLE* HashTable, int Index, char** Key);

// It clears the entire Hashtable, freeing
// the memory allocated for every key, setting
// every pointer to NULL, just as it happens when
// the Hashtable is created
int HtClear(CC_HASH_TABLE* HashTable); 

// It returns the number of entries
// of the hashtable
int HtGetKeyCount(CC_HASH_TABLE* HashTable);  


#endif _CC_HASHTABLE_H_