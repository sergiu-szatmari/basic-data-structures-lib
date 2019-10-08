#ifndef _CC_VECTOR_H_
#define _CC_VECTOR_H_

#include <stdlib.h>
#include <stdio.h>

#include "ccreturn.h"

typedef struct _CC_VECTOR 
{ 
	int* Elements;
	int Size;
	int Capacity;
} CC_VECTOR; 
  
int VecCreate(CC_VECTOR** Vector); 
int VecDestroy(CC_VECTOR** Vector); 
  
int VecInsertTail(CC_VECTOR* Vector, int Value); 
int VecInsertHead(CC_VECTOR* Vector, int Value); 
int VecInsertAfterIndex(CC_VECTOR* Vector, int Index, int Value); 
int VecRemoveByIndex(CC_VECTOR* Vector, int Index);
int VecGetValueByIndex(CC_VECTOR* Vector, int Index, int* Value);
int VecGetCount(CC_VECTOR* Vector);
int VecClear(CC_VECTOR* Vector);
int VecSort(CC_VECTOR* Vector);

int VecPrint(CC_VECTOR* Vector);

#endif _CC_VECTOR_H