#ifndef _CC_HEAP_
#define _CC_HEAP_

#include "ccvector.h"

#define MAX_HEAP 101
#define MIN_HEAP 100
#define LEFT_CHILD(i) (2 * (i) + 1)
#define RIGHT_CHILD(i) (2 * (i) + 2)
#define PARENT(i) (((i) - 1)/2)

typedef struct _CC_HEAP 
{
	CC_VECTOR* Elements;
	int Type;
} CC_HEAP;

// HpCreateMaxHeap and HpCreateMinHeap should create a max heap or a min heap, 
// respectively. InitialElements is an optional parameter and, if it is not null, the constructed
// heap should initially contain all the elements in the provided vector.
int HpCreateMaxHeap(CC_HEAP** MaxHeap, CC_VECTOR* InitialElements);
int HpCreateMinHeap(CC_HEAP** MinHeap, CC_VECTOR* InitialElements);
int HpDestroy(CC_HEAP** Heap);

// Insers a Value in a heap
int HpInsert(CC_HEAP* Heap, int Value);

// HpRemove should remove all elements with the value Value in the heap
int HpRemove(CC_HEAP* Heap, int Value);

// HpGetExtreme should return the maximum/minumum value in the heap, depending on the 
// type of heap constructed
int HpGetExtreme(CC_HEAP* Heap, int* ExtremeValue);

// HpPopExtreme should return the maximum/minimum value in the heap, and remove all 
// instances of said value from the heap
int HpPopExtreme(CC_HEAP* Heap, int* ExtremeValue);

// Returns the number of elements 
int HpGetElementCount(CC_HEAP* Heap);

// Inserts the values of the heap in ascending order in 
// a vector.
// The vector must be created before calling the function
int HpSortToVector(CC_HEAP* Heap, CC_VECTOR* SortedVector);

// Prints the Heap
int HpPrint(CC_HEAP* Heap);

#endif _CC_HEAP_