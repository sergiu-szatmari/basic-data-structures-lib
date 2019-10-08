#ifndef _CC_TREE_H_
#define _CC_TREE_H_

#include "ccstack.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct _CC_TREE {
	Node* Head;
	int Size;
} CC_TREE;

int TreeCreate(CC_TREE** Tree);
int TreeDestroy(CC_TREE** Tree);
int TreeInsert(CC_TREE* Tree, int Value);

int TreeGetCount(CC_TREE* Tree);

int TreeContains(CC_TREE* Tree, int Value);
int TreeGetHeight(CC_TREE* Tree);

int TreeRemove(CC_TREE* Tree, int Value);


int TreeClear(CC_TREE *Tree);
int TreeGetNthPreorder(CC_TREE *Tree, int Index, int *Value);


int TreeGetNthInorder(CC_TREE *Tree, int Index, int *Value);

int TreeGetNthPostorder(CC_TREE *Tree, int Index, int *Value);


void srd(Node* Head);


#endif _CC_TREE_H_