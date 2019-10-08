#ifndef _CC_LIST_H_
#define _CC_LIST_H_

#include <stdlib.h>

#define FIRST_NODE_VALUE -999

#include "ccreturn.h"

typedef struct _CC_LIST_ENTRY 
{
	struct _CC_LIST_ENTRY* PrevNode;
	struct _CC_LIST_ENTRY* NextNode;
	int Value;
} CC_LIST_ENTRY;

// Creates the linked list
int LstCreate(CC_LIST_ENTRY** List);

// Destroys the linked list
int LstDestroy(CC_LIST_ENTRY** List);

// Insers a value at the head of the list
int LstInsertValue(CC_LIST_ENTRY* List, int Value);

// Removes a given node from the List. If the
// node does not exists, the result of the 
// function is "-2" (INCORRECT_PARAMETER)
int LstRemoveNode(CC_LIST_ENTRY* List, CC_LIST_ENTRY* Node);

// The funtion sets a certain node's value in the
// variable Value. If the node does not exist, 
// the function returns a negative integer.
int LstGetNodeValue(CC_LIST_ENTRY* List, CC_LIST_ENTRY* Node, int* Value);

// Sets the Nth node's address in the "Node" parameter
// if it exists, NULL it if doesn't
int LstGetNthNode(CC_LIST_ENTRY* List, int Index, CC_LIST_ENTRY** Node);

// It counts how many elements the list has
int LstGetNodeCount(CC_LIST_ENTRY* List);

// It clears the list, freeing all the memory
// allocated for every Node, except the first 
// one, the head of the list
int LstClear(CC_LIST_ENTRY* List);

// It sorts the list
int LstSortByValues(CC_LIST_ENTRY* List);

// The two lists are merged into the "Destination"
// one, w/o the duplicated elements
int LstMergeSortedLists(CC_LIST_ENTRY* Destination, CC_LIST_ENTRY* Source);

// Prints the list
int LstPrint(CC_LIST_ENTRY* List);

#endif _CC_LIST_H_