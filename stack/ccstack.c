#pragma once

#include "ccstack.h"

int NdCreate(Node** newNode)
{
	if ((*newNode) != NULL)
		return INVALID_PARAMETER;

	(*newNode) = (Node*)malloc(sizeof(Node));
	(*newNode)->LeftChild = NULL;
	(*newNode)->RightChild = NULL;
	(*newNode)->Parent = NULL;
	(*newNode)->Value = -999;
	(*newNode)->LeftHeight = 0;
	(*newNode)->RightHeight = 0;
	(*newNode)->Count = 1;

	return SUCCESS;
}

int StCreate(CC_STACK** Stack)
{
	if ((*Stack) != NULL)
		return INVALID_PARAMETER;

	(*Stack) = (CC_STACK*)malloc(sizeof(CC_STACK));

	(*Stack)->Size = 0;
	(*Stack)->Capacity = 10;
	(*Stack)->Elements = (Node*)malloc(sizeof(Node) * 10);

	if ((*Stack) == NULL)
		return MALLOC_FAILURE;

	return SUCCESS;
}



int StDestroy(CC_STACK** Stack)
{
	if ((*Stack) == NULL)
		return INVALID_PARAMETER;

	free((*Stack)->Elements);
	free((*Stack));

	return SUCCESS;
}



int StIsEmpty(CC_STACK* stack)
{
	if (stack->Size == 0)
		return CC_TRUE;
	return CC_FALSE;
}



int StPush(CC_STACK* Stack, const Node* Elem)
{
	if (Stack == NULL)
		return INVALID_PARAMETER;

	if (Stack->Size == Stack->Capacity) {
		Stack->Capacity = Stack->Capacity * 2;
		Node* NewElements = (Node*)malloc(Stack->Capacity * sizeof(Node));

		for (int i = 0; i < Stack->Size; i++)
			NewElements[i] = Stack->Elements[i];
		free(Stack->Elements);
		Stack->Elements = NewElements;
	}
	Stack->Elements[Stack->Size++] = *Elem;

	return SUCCESS;
}



int StPop(CC_STACK* stack, Node** PopNode)
{
	if (stack == NULL)
		return INVALID_PARAMETER;

	if (stack->Size == 0)
		return INCORRECT_PARAMETER;

	(*PopNode) = stack->Elements + stack->Size - 1;
	stack->Size--;

	return SUCCESS;
}

