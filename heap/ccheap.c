#include "ccheap.h"

int HeapifyUp(CC_HEAP* Hp, int Index)
{
	if (Hp == NULL)
		return INVALID_PARAMETER;

	if (Index < 0)
		return INCORRECT_PARAMETER;

	if (Index >= VecGetCount(Hp->Elements))
		return INCORRECT_PARAMETER;

	int PlacedRight = 0;
	
	if (Hp->Type == MAX_HEAP)
		while (PARENT(Index) >= 0 && Hp->Elements->Elements[PARENT(Index)] < Hp->Elements->Elements[Index])
		{
			int aux;
			aux = Hp->Elements->Elements[PARENT(Index)];
			Hp->Elements->Elements[PARENT(Index)] = Hp->Elements->Elements[Index];
			Hp->Elements->Elements[Index] = aux;
			Index = PARENT(Index);
		}
	else if (Hp->Type == MIN_HEAP)
		while (PARENT(Index) >= 0 && Hp->Elements->Elements[PARENT(Index)] > Hp->Elements->Elements[Index])
		{
			int aux;
			aux = Hp->Elements->Elements[PARENT(Index)];
			Hp->Elements->Elements[PARENT(Index)] = Hp->Elements->Elements[Index];
			Hp->Elements->Elements[Index] = aux;
			Index = PARENT(Index);
		}
	return SUCCESS;
}

int HpCreateMaxHeap(CC_HEAP** MaxHeap, CC_VECTOR* InitialElements)
{
	if (MaxHeap == NULL)
		return INVALID_PARAMETER;

//	if (*MaxHeap == NULL)
//		return INVALID_PARAMETER;

	(*MaxHeap) = (CC_HEAP*)malloc(sizeof(CC_HEAP));

	if ((*MaxHeap) == NULL)
		return MALLOC_FAILURE;

	(*MaxHeap)->Type = MAX_HEAP;
	(*MaxHeap)->Elements = NULL;
		
	if (VecCreate(&((*MaxHeap)->Elements)) < 0)
		return EXEC_FAILURE;

	if (InitialElements != NULL)
	{
		int i;
		int count = VecGetCount(InitialElements);
		for (i = 0; i < count; i++)
		{
			int Elem;
			if (VecGetValueByIndex(InitialElements, i, &Elem) < 0)
				return EXEC_FAILURE;
			if (VecInsertTail((*MaxHeap)->Elements, Elem) < 0)
				return EXEC_FAILURE;
			HeapifyUp((*MaxHeap), i);
		}
		
	}

	return SUCCESS;

}

int HpCreateMinHeap(CC_HEAP** MinHeap, CC_VECTOR* InitialElements)
{
	if (MinHeap == NULL)
		return INVALID_PARAMETER;

//	if (*MinHeap == NULL)
//		return INVALID_PARAMETER;

	(*MinHeap) = (CC_HEAP*)malloc(sizeof(CC_HEAP));

	if ((*MinHeap) == NULL)
		return MALLOC_FAILURE;

	(*MinHeap)->Type = MIN_HEAP;

	CC_VECTOR* Vector = NULL;
	if (VecCreate(&Vector) < 0)
		return EXEC_FAILURE;

	(*MinHeap)->Elements = Vector;

	if (InitialElements != NULL)
	{
		int i;
		int Count;
		Count = VecGetCount(InitialElements);
		for (i = 0; i < Count; i++)
		{
			int Element;
			if (VecGetValueByIndex(InitialElements, i, &Element) < 0)
				return EXEC_FAILURE;
			if (VecInsertTail((*MinHeap)->Elements, Element) < 0)
				return EXEC_FAILURE;
			HeapifyUp((*MinHeap), i);
		}
	}

	return SUCCESS;
}

int HpDestroy(CC_HEAP** Heap)
{
	if (Heap == NULL)
		return INVALID_PARAMETER;

	if (*Heap == NULL)
		return INVALID_PARAMETER;

	if (VecDestroy(&((*Heap)->Elements)) < 0)
		return EXEC_FAILURE;

	free((*Heap));

	return SUCCESS;
}

int HpInsert(CC_HEAP* Heap, int Value)
{
	if (Heap == NULL)
		return INVALID_PARAMETER;

	VecInsertTail(Heap->Elements, Value);
	HeapifyUp(Heap, Heap->Elements->Size - 1);

	return SUCCESS;
}

int HpRemove(CC_HEAP* Heap, int Value)
{
	if (Heap == NULL)
		return INVALID_PARAMETER;

	CC_HEAP* AuxHeap = NULL;
	if (Heap->Type == MAX_HEAP)
		if (HpCreateMaxHeap(&AuxHeap, NULL) < 0)
			return EXEC_FAILURE;
		else;
	else
		if (HpCreateMinHeap(&AuxHeap, NULL) < 0)
			return EXEC_FAILURE;

	int i;
	for (i = 0; i < Heap->Elements->Size; i++)
		if (Heap->Elements->Elements[i] != Value)
			HpInsert(AuxHeap, Heap->Elements->Elements[i]);

	
	VecDestroy(&(Heap->Elements));
	CC_VECTOR* Vec = NULL;
	VecCreate(&Vec);
	for (i = 0; i < AuxHeap->Elements->Size; i++)
		VecInsertTail(Vec, AuxHeap->Elements->Elements[i]);
	Heap->Elements = Vec;

	HpDestroy(&AuxHeap);

	return SUCCESS;

}

int HpGetExtreme(CC_HEAP* Heap, int* ExtremeValue)
{
	if (Heap == NULL)
		return INVALID_PARAMETER;

	if (ExtremeValue == NULL)
		return INVALID_PARAMETER;

	*ExtremeValue = Heap->Elements->Elements[0];

	return SUCCESS;
}

int HpPopExtreme(CC_HEAP* Heap, int* ExtremeValue)
{
	if (Heap == NULL)
		return INVALID_PARAMETER;

	if (ExtremeValue == NULL)
		return INVALID_PARAMETER;

	if (HpGetExtreme(Heap, ExtremeValue) < 0)
		return EXEC_FAILURE;

	if (HpRemove(Heap, *ExtremeValue) < 0)
		return EXEC_FAILURE;

	return SUCCESS;

}

int HpGetElementCount(CC_HEAP* Heap)
{
	if (Heap == NULL)
		return INVALID_PARAMETER;

	return VecGetCount(Heap->Elements);
}

int HpSortToVector(CC_HEAP* Heap, CC_VECTOR* SortedVector)
{
	if (Heap == NULL)
		return INVALID_PARAMETER;

	if (SortedVector == NULL)
		return INVALID_PARAMETER;

	CC_HEAP* Aux = NULL;

	if (Heap->Type == MIN_HEAP)
	{
		if (HpCreateMinHeap(&Aux, Heap->Elements) < 0)
			return EXEC_FAILURE;
		while (HpGetElementCount(Aux) > 0)
		{
			int InitialCount = HpGetElementCount(Aux);
			int value, times, i;
			if (HpPopExtreme(Aux, &value) < 0) return EXEC_FAILURE;
			times = InitialCount - HpGetElementCount(Aux);
			for (i = 0; i < times; i++)
				if (VecInsertTail(SortedVector, value) < 0) return EXEC_FAILURE;
		}
	}
	else if (Heap->Type == MAX_HEAP)
	{
		if (HpCreateMaxHeap(&Aux, Heap->Elements) < 0)
			return EXEC_FAILURE;
		while (HpGetElementCount(Aux) > 0)
		{
			int InitialCount = HpGetElementCount(Aux);
			int value, times, i;
			if (HpPopExtreme(Aux, &value) < 0) return EXEC_FAILURE;
			times = InitialCount - HpGetElementCount(Aux);
			for (i = 0; i < times; i++)
				if (VecInsertHead(SortedVector, value) < 0) return EXEC_FAILURE;
		}
	}
	HpDestroy(&Aux);
	return SUCCESS;
}

int HpPrint(CC_HEAP * Heap)
{
	if (Heap == NULL)
		return INVALID_PARAMETER;

	int i;
	for (i = 0; i < Heap->Elements->Size; i++)
		printf("%d ", Heap->Elements->Elements[i]);
	printf("\n");
	return SUCCESS;
}
