#ifndef _CC_STACK_H_
#define _CC_STACK_H_

#include <stdlib.h>
#include <stdio.h>

#include "ccvector.h"

#define CC_TRUE 1
#define CC_FALSE 0

typedef struct Node 
{
	struct Node* LeftChild;
	struct Node* RightChild;
	struct Node* Parent;
	int Value, LeftHeight, RightHeight;
	int Count;
} Node;

typedef struct
{
	Node* Elements;
	int Size;
	int Capacity;

} CC_STACK;

int NdCreate(Node** newNode);

int StCreate(CC_STACK** Stack);

int StDestroy(CC_STACK** Stack);

int StIsEmpty(CC_STACK* Stack);

int StPush(CC_STACK* Stack, const Node* Elem);

int StPop(CC_STACK* stack, Node** PopNode);

#endif _CC_STACK_H_