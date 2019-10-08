#include "cclist.h"
#include "ccvector.h"

int LstCreate(CC_LIST_ENTRY** List)
{
	if (List == NULL)
		return INVALID_PARAMETER;

	*List = (CC_LIST_ENTRY*)malloc(sizeof(CC_LIST_ENTRY));

	if ((*List) == NULL)
		return MALLOC_FAILURE;

	(*List)->PrevNode = NULL;
	(*List)->NextNode = NULL;
	(*List)->Value = FIRST_NODE_VALUE;

	return SUCCESS;
}

int LstDestroy(CC_LIST_ENTRY** List)
{
	if (List == NULL)
		return INVALID_PARAMETER;

	if (*List == NULL)
		return INVALID_PARAMETER;

	if ((*List)->PrevNode != NULL)
		return INCORRECT_PARAMETER;

	CC_LIST_ENTRY* CurrentNode = (*List)->NextNode;

	while (CurrentNode != NULL)
	{
		CC_LIST_ENTRY* Aux = CurrentNode;
		CurrentNode = CurrentNode->NextNode;
		free(Aux);
	}
	
	free(*List);

	return SUCCESS;
}

int LstInsertValue(CC_LIST_ENTRY* List, int Value)
{
	if (List == NULL)
		return INVALID_PARAMETER;

	if (List->PrevNode != NULL)
		return INCORRECT_PARAMETER;

	CC_LIST_ENTRY* newNode;
	if (LstCreate(&newNode) < 0)
		return EXEC_FAILURE;

	newNode->Value = Value;

	newNode->NextNode = List->NextNode;
	if (List->NextNode == NULL)
	{
		List->NextNode = newNode;
		newNode->PrevNode = List;
	}
	else
	{
		List->NextNode->PrevNode = newNode;
		List->NextNode = newNode;
	}

	return SUCCESS;
}

int LstRemoveNode(CC_LIST_ENTRY* List, CC_LIST_ENTRY* Node)
{
	if (List == NULL)
		return INVALID_PARAMETER;

	if (Node == NULL)
		return INVALID_PARAMETER;

	if (List->NextNode == NULL)
		return INVALID_PARAMETER;

	if (List->PrevNode != NULL)
		return INCORRECT_PARAMETER;

	if (List == Node)
		return INCORRECT_PARAMETER;

	if (List->NextNode == Node)
	{
		CC_LIST_ENTRY* aux = List->NextNode;
		List->NextNode = aux->NextNode;
		aux->NextNode->PrevNode = List;
		free(aux);
	}
	else
	{
		CC_LIST_ENTRY* CurrentNode = List->NextNode;
		while (CurrentNode->NextNode != Node && CurrentNode != NULL)
			CurrentNode = CurrentNode->NextNode;

		if (CurrentNode == NULL)
			return INCORRECT_PARAMETER;

		if (CurrentNode->NextNode != Node ||
			CurrentNode->NextNode->NextNode != Node->NextNode ||
			CurrentNode->NextNode->PrevNode != Node->PrevNode ||
			CurrentNode->NextNode->Value != Node->Value)
			return EXEC_FAILURE;

		CC_LIST_ENTRY* Aux = CurrentNode->NextNode;
		CurrentNode->NextNode = Aux->NextNode;
		Aux->NextNode->PrevNode = CurrentNode;
		free(Aux);
	}

	return SUCCESS;
}

int LstGetNodeValue(CC_LIST_ENTRY* List, CC_LIST_ENTRY* Node, int* Value)
{
	if (List == NULL)
		return INVALID_PARAMETER;

	if (List->PrevNode != NULL)
		return INCORRECT_PARAMETER;

	if (Node == NULL)
		return INVALID_PARAMETER;

	if (Value == NULL)
		return INVALID_PARAMETER;

	if (List == Node)
		return INCORRECT_PARAMETER;

	CC_LIST_ENTRY* CurrentNode = List->NextNode;

	if (CurrentNode == NULL)
		return INCORRECT_PARAMETER;

	while (CurrentNode != NULL && CurrentNode != Node)
		CurrentNode = CurrentNode->NextNode;

	if (CurrentNode == NULL)
		return EXEC_FAILURE;

	*Value = CurrentNode->Value;

	return SUCCESS;
}

int LstGetNthNode(CC_LIST_ENTRY* List, int Index, CC_LIST_ENTRY** Node)
{
	if (List == NULL)
		return INVALID_PARAMETER;

	if (Index < 0)
		return INCORRECT_PARAMETER;

	if (Node == NULL)
		return INVALID_PARAMETER;

	if ((*Node) != NULL)
		return INCORRECT_PARAMETER;

	CC_LIST_ENTRY* CurrentNode = List->NextNode;

	if (CurrentNode == NULL)
		return INCORRECT_PARAMETER;

	int CurrentIndex = 0;
	while (CurrentNode != NULL && CurrentIndex != Index)
	{
		CurrentNode = CurrentNode->NextNode;
		CurrentIndex++;
	}

	*Node = CurrentNode;

	return SUCCESS;
}

int LstGetNodeCount(CC_LIST_ENTRY* List)
{
	if (List == NULL)
		return INVALID_PARAMETER;

	if (List->PrevNode != NULL)
		return INCORRECT_PARAMETER;

	int Count = 0;

	CC_LIST_ENTRY* CurrentNode = List->NextNode;

	while (CurrentNode != NULL)
	{
		Count++;
		CurrentNode = CurrentNode->NextNode;
	}

	return Count;
}

int LstClear(CC_LIST_ENTRY* List)
{
	if (List == NULL)
		return INVALID_PARAMETER;

	if (List->PrevNode != NULL)
		return INCORRECT_PARAMETER;

	if (List->NextNode != NULL)
	// Lista nu e vida deja
	{
		CC_LIST_ENTRY* Node = List->NextNode;
		while (Node != NULL) 
		{
			CC_LIST_ENTRY* Aux = Node;
			Node = Node->NextNode;
			free(Aux);
		}
		List->NextNode = NULL;
		List->PrevNode = NULL;
	}

	return SUCCESS;
}

int LstSortByValues(CC_LIST_ENTRY* List)
{
	if (List == NULL)
		return INVALID_PARAMETER;

	if (List->PrevNode != NULL)
		return INCORRECT_PARAMETER;

	CC_VECTOR* Vector = NULL;
	if (VecCreate(&Vector) < 0)
	{
		printf("VecCreate error\n");
		return EXEC_FAILURE;
	}

	CC_LIST_ENTRY* CurrentNode = List->NextNode;

	while (CurrentNode != NULL)
	{
		if (VecInsertTail(Vector, CurrentNode->Value) < 0)
		{
			printf("VecInsertTail error\n");
			return EXEC_FAILURE;
		}		
		CurrentNode = CurrentNode->NextNode;
	}

	if (VecSort(Vector) < 0)
	{
		printf("VecSort error\n");
		return EXEC_FAILURE;
	}

	CurrentNode = List->NextNode;
	int Index = 0;
	while (CurrentNode != NULL)
	{
		CurrentNode->Value = Vector->Elements[Index++];
		CurrentNode = CurrentNode->NextNode;
	}

	if (VecDestroy(&Vector) < 0)
	{
		printf("VecDestroy error\n");
		return EXEC_FAILURE;
	}

	return SUCCESS;
}

int LstMergeSortedLists(CC_LIST_ENTRY* Destination, CC_LIST_ENTRY* Source)
{
	if (Destination == NULL)
		return INVALID_PARAMETER;

	if (Source == NULL)
		return INVALID_PARAMETER;

	if (Destination->PrevNode != NULL)
		return INCORRECT_PARAMETER;

	if (Source->PrevNode != NULL)
		return INCORRECT_PARAMETER;
	
	CC_LIST_ENTRY* DestCurrentNode = Destination;
	CC_LIST_ENTRY* SrcCurrentNode = Source->NextNode;

	while (DestCurrentNode->NextNode != NULL && SrcCurrentNode != NULL)
	{
		if (DestCurrentNode->NextNode->Value < SrcCurrentNode->Value)
			DestCurrentNode = DestCurrentNode->NextNode;
		else if (DestCurrentNode->NextNode->Value > SrcCurrentNode->Value)
		{
			while (SrcCurrentNode != NULL && DestCurrentNode->NextNode != NULL &&
				!(DestCurrentNode->NextNode->Value < SrcCurrentNode->Value))
			{
				CC_LIST_ENTRY* newNode = NULL;
				LstCreate(&newNode);
				newNode->Value = SrcCurrentNode->Value;
				newNode->PrevNode = DestCurrentNode;
				newNode->NextNode = DestCurrentNode->NextNode;
				DestCurrentNode->NextNode = newNode;
				newNode->NextNode->PrevNode = newNode;

				SrcCurrentNode = SrcCurrentNode->NextNode;
				//DestCurrentNode = DestCurrentNode->NextNode;
				//DestCurrentNode = DestCurrentNode->NextNode;
			}
		}
		else
			SrcCurrentNode = SrcCurrentNode->NextNode;
	}

	while (SrcCurrentNode != NULL)
	{
		CC_LIST_ENTRY* newNode;
		LstCreate(&newNode);

		newNode->Value = SrcCurrentNode->Value;
		newNode->NextNode = DestCurrentNode->NextNode;
		newNode->PrevNode = DestCurrentNode;
		DestCurrentNode->NextNode = newNode;

		DestCurrentNode = DestCurrentNode->NextNode;
		SrcCurrentNode = SrcCurrentNode->NextNode;

	}

	return SUCCESS;
}

int LstPrint(CC_LIST_ENTRY* List)
{
	if (List == NULL)
		return -1;

	CC_LIST_ENTRY* CurrentNode = List->NextNode;
	if (CurrentNode == NULL)
	{
		printf("Lista este vida\n\n");
		return -1;
	}
	
	while (CurrentNode != NULL)
	{
		printf("%d --> ", CurrentNode->Value);
		CurrentNode = CurrentNode->NextNode;
	}
	printf("X\n");

	return 0;
}
