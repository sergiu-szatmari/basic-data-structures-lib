#include "ccvector.h"

int VecResize(CC_VECTOR* Vector, int newCapacity)
{
	if (Vector == NULL)
		return INVALID_PARAMETER;

	if (newCapacity < 0)
		return INCORRECT_PARAMETER;

	if (newCapacity < Vector->Capacity)
		return INCORRECT_PARAMETER;

	int* newElems = (int*)malloc(sizeof(int) * newCapacity);

	if (newElems == NULL)
		return MALLOC_FAILURE;

	int i;
	for (i = 0; i < Vector->Size; i++)
		newElems[i] = Vector->Elements[i];

	free(Vector->Elements);
	Vector->Elements = newElems;
	Vector->Capacity = newCapacity;

	return SUCCESS;
}

int VecDownsize(CC_VECTOR* Vector, int newCapacity)
{
	if (Vector == NULL)
		return INVALID_PARAMETER;

	if (newCapacity < 0)
		return INCORRECT_PARAMETER;

	if (newCapacity > Vector->Capacity)
		return INCORRECT_PARAMETER;

	int* newElems = (int*)malloc(sizeof(int) * newCapacity);

	if (newElems == NULL)
		return MALLOC_FAILURE;

	int i;
	for (i = 0; i < newCapacity; i++)
		newElems[i] = Vector->Elements[i];

	free(Vector->Elements);
	Vector->Elements = newElems;
	Vector->Capacity = newCapacity;

	return SUCCESS;
}

int VecPrint(CC_VECTOR* Vector)
{
	if (Vector == NULL)
		return INVALID_PARAMETER;

	if (Vector->Size == 0)
		printf("Vectorul nu are niciun element\n\n");
	else
	{
		int i;
		printf("Vector: ");
		for (i = 0; i < Vector->Size - 1; i++)
			printf("%d ", Vector->Elements[i]);
		printf("%d\n\n", Vector->Elements[Vector->Size - 1]);
	}
	return SUCCESS;
}

int VecCreate(CC_VECTOR** Vector)
{
	if (Vector == NULL)
		return INVALID_PARAMETER;

	if ((*Vector) != NULL)
		return INVALID_PARAMETER;
	
	(*Vector) = (CC_VECTOR*)malloc(sizeof(CC_VECTOR));
	
	if (*Vector == NULL)
		return INVALID_PARAMETER;

	(*Vector)->Size = 0;
	(*Vector)->Capacity = 10;
	(*Vector)->Elements = (int*)malloc(sizeof(int) * (*Vector)->Capacity);

	if ((*Vector)->Elements == NULL)
		return MALLOC_FAILURE;

	return SUCCESS;
}

int VecDestroy(CC_VECTOR** Vector)
{
	if (Vector == NULL)
		return INVALID_PARAMETER;

	if ((*Vector) == NULL)
		return INVALID_PARAMETER;

	free((*Vector)->Elements);
	free(*Vector);

	return SUCCESS;
}

int VecInsertTail(CC_VECTOR* Vector, int Value)
{
	if (Vector == NULL)
		return INVALID_PARAMETER;

	if (Vector->Size == Vector->Capacity)
		if (VecResize(Vector, Vector->Capacity * 2) < 0)
			return EXEC_FAILURE;

	Vector->Elements[Vector->Size] = Value;
	Vector->Size += 1;

	return SUCCESS;
}

int VecInsertHead(CC_VECTOR* Vector, int Value)
{
	if (Vector == NULL)
		return INVALID_PARAMETER;

	if (Vector->Size + 1 == Vector->Capacity)
		if (VecResize(Vector, Vector->Capacity * 2) < 0)
			return EXEC_FAILURE;

	int i;
	Vector->Size++;
	for (i = Vector->Size; i > 0; i--)
		Vector->Elements[i] = Vector->Elements[i - 1];
	
	Vector->Elements[0] = Value;

	return SUCCESS;
}

int VecInsertAfterIndex(CC_VECTOR* Vector, int Index, int Value)
{
	if (Vector == NULL)
		return INVALID_PARAMETER;

	if (Index < 0)
		return INCORRECT_PARAMETER;

	if (Index >= Vector->Size)
		return INCORRECT_PARAMETER;

	if (Vector->Size == Vector->Capacity)
		if (VecResize(Vector, Vector->Capacity * 2) < 0)
			return EXEC_FAILURE;

	int i;
	Vector->Size += 1;
	for (i = Vector->Size; i > Index + 1; i--)
		Vector->Elements[i] = Vector->Elements[i - 1];

	Vector->Elements[Index + 1] = Value;

	return SUCCESS;
}

int VecRemoveByIndex(CC_VECTOR* Vector, int Index)
{
	if (Vector == NULL)
		return INVALID_PARAMETER;

	if (Index < 0)
		return INCORRECT_PARAMETER;

	if (Index >= Vector->Size)
		return INCORRECT_PARAMETER;

	int i;
	for (i = Index; i < Vector->Size - 1; i++)
		Vector->Elements[i] = Vector->Elements[i + 1];
	Vector->Size -= 1;

	if (Vector->Size < (Vector->Capacity / 4))
		if (VecDownsize(Vector, Vector->Capacity / 2) < 0)
			return EXEC_FAILURE;

	return SUCCESS;
}

int VecGetValueByIndex(CC_VECTOR* Vector, int Index, int* Value)
{
	if (Vector == NULL)
		return INVALID_PARAMETER;

	if (Index < 0)
		return INCORRECT_PARAMETER;

	if (Index >= Vector->Size)
		return INCORRECT_PARAMETER;

	if (Value == NULL)
		return INVALID_PARAMETER;

	*Value = Vector->Elements[Index];

	return SUCCESS;
}

int VecGetCount(CC_VECTOR* Vector)
{
	if (Vector == NULL)
		return INVALID_PARAMETER;

	return Vector->Size;
}

int VecClear(CC_VECTOR* Vector)
{
	if (Vector == NULL)
		return INVALID_PARAMETER;

	VecDownsize(Vector, 10);

	Vector->Size = 0;

	return SUCCESS;
}

int MergeVectors(CC_VECTOR* Vector1, CC_VECTOR* Vector2, CC_VECTOR* ResultVector)
{
	if (Vector1 == NULL)
		return INVALID_PARAMETER;

	if (Vector2 == NULL)
		return INVALID_PARAMETER;

	if (ResultVector == NULL)
		return INVALID_PARAMETER;

	int i = 0, j = 0, k = 0;

	while (i < Vector1->Size && j < Vector2->Size)
	{
		if (Vector1->Elements[i] < Vector2->Elements[j])
			ResultVector->Elements[k++] = Vector1->Elements[i++];
		else
			ResultVector->Elements[k++] = Vector2->Elements[j++];
	}

	while (i < Vector1->Size)
		ResultVector->Elements[k++] = Vector1->Elements[i++];

	while (j < Vector2->Size)
		ResultVector->Elements[k++] = Vector2->Elements[j++];

	return SUCCESS;
}

int MergeSort(CC_VECTOR* Vector)
{
	int i, Middle;
	CC_VECTOR* LeftVec = NULL;
	CC_VECTOR* RightVec = NULL;

	if (Vector->Size < 2)
		return SUCCESS;

	if (VecCreate(&LeftVec) < 0)
		return EXEC_FAILURE;

	if (VecCreate(&RightVec) < 0)
		return EXEC_FAILURE;

	Middle = Vector->Size / 2;
	for (i = 0; i < Middle; i++)
		if (VecInsertTail(LeftVec, Vector->Elements[i]) < 0)
			return EXEC_FAILURE;

	for (i = Middle; i < Vector->Size; i++)
		if (VecInsertTail(RightVec, Vector->Elements[i]) < 0)
			return EXEC_FAILURE;
	
	if (MergeSort(LeftVec) < 0)
		return EXEC_FAILURE;

	if (MergeSort(RightVec) < 0)
		return EXEC_FAILURE;

	if (MergeVectors(LeftVec, RightVec, Vector) < 0)
		return EXEC_FAILURE;
	
	if (VecDestroy(&LeftVec) < 0)
		return EXEC_FAILURE;

	if (VecDestroy(&RightVec) < 0)
		return EXEC_FAILURE;

	return SUCCESS;
}

int VecSort(CC_VECTOR* Vector)
{
	if (Vector == NULL)
		return INVALID_PARAMETER;

	if (MergeSort(Vector) < 0)
		return EXEC_FAILURE;

	return SUCCESS;
}
