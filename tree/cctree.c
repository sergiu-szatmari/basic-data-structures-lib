
#include "cctree.h"
#include "ccvector.h"

// For Balancing
void _SetLeftRight_(Node* Node, CC_TREE* Tree) 
{

	while (Node->Parent != NULL)
	{
		if (Node->Value > Node->Parent->Value)
		{
			Node->Parent->RightHeight = MAX(Node->Parent->RightHeight, 1 + MAX(Node->RightHeight, Node->LeftHeight));
			Node = Node->Parent;
		}
		else
		{
			Node->Parent->LeftHeight = MAX(Node->Parent->LeftHeight, 1 + MAX(Node->RightHeight, Node->LeftHeight));
			Node = Node->Parent;
		}
	}

}

Node* _RotateLeft_(Node* Head) 
{
	Node* NewHead = Head->RightChild;

	Head->RightChild = NewHead->LeftChild;
	NewHead->LeftChild = Head;
	NewHead->Parent = Head->Parent;
	Head->Parent = NewHead;

	if (Head->RightChild == NULL)
		Head->RightHeight = 0;
	else
		Head->RightHeight = 1 + MAX(Head->RightChild->RightHeight, Head->RightChild->LeftHeight);

	if (Head->RightChild != NULL)
		Head->RightChild->Parent = Head;

	NewHead->LeftHeight = 1 + MAX(Head->LeftHeight, Head->RightHeight);
	return NewHead;
}

Node* _RotateRight_(Node* Head) 
{
	Node* NewHead = Head->LeftChild;

	Head->LeftChild = NewHead->RightChild;
	NewHead->RightChild = Head;
	NewHead->Parent = Head->Parent;
	Head->Parent = NewHead;

	if (Head->LeftChild == NULL)
		Head->LeftHeight = 0;
	else
		Head->LeftHeight = 1 + MAX(Head->LeftChild->RightHeight, Head->LeftChild->LeftHeight);

	if (Head->LeftChild != NULL)
		Head->LeftChild->Parent = Head;

	NewHead->RightHeight = 1 + MAX(Head->RightHeight, Head->LeftHeight);
	return NewHead;
}

int _IsLeftHeavy_(Node* Head) {	return Head->LeftHeight > Head->RightHeight; }

Node* _Balance_(Node* Head) 
{
	if (Head->LeftHeight - Head->RightHeight <= 1 && Head->RightHeight - Head->LeftHeight <= 1)
		return Head;

	if (!_IsLeftHeavy_(Head)) 
	{
		if (_IsLeftHeavy_(Head->RightChild))
			Head->RightChild = _RotateRight_(Head->RightChild);
		return _RotateLeft_(Head);
	}
	else 
	{
		if (!_IsLeftHeavy_(Head->LeftChild))
			Head->LeftChild = _RotateLeft_(Head->LeftChild);
		return _RotateRight_(Head);
	}
}

Node* _BalanceTree_(Node* Head) 
{

	if (Head == NULL)
		return NULL;

	while (Head->Parent != NULL) 
	{
		Head = _Balance_(Head);

		if (Head->Parent->Value > Head->Value) 
		{
			Head->Parent->LeftChild = Head;
			Head->Parent->LeftHeight = 1 + MAX(Head->RightHeight, Head->LeftHeight);
			Head = Head->Parent;
		}
		else 
		{
			Head->Parent->RightChild = Head;
			Head->Parent->RightHeight = 1 + MAX(Head->RightHeight, Head->LeftHeight);
			Head = Head->Parent;
		}

	}
	return _Balance_(Head);

}

Node* _GetNode_(CC_TREE* Tree, int Value) 
{
	if (Tree == NULL)
		return NULL;

	Node* Head = Tree->Head;

	while (Head != NULL) 
	{
		if (Head->Value == Value)
			return Head;
		if (Head->Value > Value)
			Head = Head->LeftChild;
		else
			Head = Head->RightChild;
	}

	return NULL;
}



// For Deleting
int _IsLeaf_(Node* Head) { return Head->RightChild == NULL && Head->LeftChild == NULL; }

int _Has2Sons_(Node* Head) { return Head->LeftChild != NULL && Head->RightChild != NULL; }

void _DeleteLeaf_(Node* Head, CC_TREE* Tree) 
{
	if (Head->Parent == NULL) 
	{
		free(Head);
		Tree->Head = NULL;
		return;
	}

	if (Head->Value < Head->Parent->Value) 
	{
		Head->Parent->LeftHeight--;
		Head->Parent->LeftChild = NULL;
	}
	else 
	{
		Head->Parent->RightHeight--;
		Head->Parent->RightChild = NULL;
	}
	_SetLeftRight_(Head->Parent, Tree);
	Tree->Head = _BalanceTree_(Head->Parent);

	free(Head);
}

void _DeleteWith1Son_(Node* Head, CC_TREE* Tree) 
{
	if (Head->Parent == NULL) 
	{
		if (Head->LeftChild != NULL) 
		{
			Head->Value = Head->LeftChild->Value;
			free(Head->LeftChild);
		}
		else 
		{
			Head->Value = Head->RightChild->Value;
			free(Head->RightChild);
		}
		Head->LeftChild = Head->RightChild = NULL;
	}

	Node* Parent = Head->Parent;
	Node* Son;

	if (Head->LeftChild != NULL)
		Son = Head->LeftChild;
	else
		Son = Head->RightChild;

	if (Son->Value < Parent->Value) 
	{
		Parent->LeftChild = Son;
		Son->Parent = Parent;
	}
	else 
	{
		Parent->RightChild = Son;
		Son->Parent = Parent;
	}

	free(Head);
	_SetLeftRight_(Son, Tree);
	Tree->Head = _BalanceTree_(Son);
}

void _DeleteWith2Sons_(Node* Head, CC_TREE* Tree) 
{
	Node* LeftChild = Head->LeftChild;

	while (LeftChild != NULL && LeftChild->RightChild != NULL)
		LeftChild = LeftChild->RightChild;

	int Temp = LeftChild->Value;
	Node* OldHead = Head;

	if (_IsLeaf_(LeftChild))
		_DeleteLeaf_(LeftChild, Tree);
	else
		_DeleteWith1Son_(LeftChild, Tree);

	OldHead->Value = Temp;
}




int TreeCreate(CC_TREE** Tree)
{
	if (Tree == NULL)
		return INVALID_PARAMETER;

	(*Tree) = (CC_TREE*)malloc(sizeof(CC_TREE));
	if (*Tree < 0)
		return MALLOC_FAILURE;

	(*Tree)->Head = NULL;
	(*Tree)->Size = 0;
	
	return SUCCESS;
}

int TreeDestroy(CC_TREE** Tree)
{
	if (Tree == NULL)
		return INVALID_PARAMETER;

	Node* Head = (*Tree)->Head;

	while (Head != NULL) {

		if (Head->LeftChild != NULL) 
		{
			Head = Head->LeftChild;
			continue;
		}
		if (Head->RightChild != NULL) 
		{
			Head = Head->RightChild;
			continue;
		}
		Node* Parent = Head->Parent;

		if (Parent != NULL) 
		{
			if (Head->Value < Parent->Value)
				Parent->LeftChild = NULL;
			else
				Parent->RightChild = NULL;
		}

		free(Head);
		Head = Parent;
	}

	(*Tree)->Head = NULL;
	free((*Tree));

	return SUCCESS;
}

int TreeInsert(CC_TREE* Tree, int Value)
{	
	if (Tree == NULL)
		return INVALID_PARAMETER;

	Node* n = _GetNode_(Tree, Value);
	if (n != NULL) {
		n->Count++;
		Tree->Size++;
		return SUCCESS;
	}


	if (Tree->Head == NULL) {
		NdCreate(&(Tree->Head));
		Tree->Head->Value = Value;
		Tree->Size++;
		return SUCCESS;
	}

	Node* Head = Tree->Head;
	Node* Last = Head;

	while (Head != NULL) {
		Last = Head;
		if (Head->Value > Value)
			Head = Head->LeftChild;
		else
			Head = Head->RightChild;
	}

	if (Last->Value > Value) {
		NdCreate(&(Last->LeftChild));
		Last->LeftChild->Value = Value;
		Last->LeftChild->Parent = Last;
		_SetLeftRight_(Last->LeftChild, Tree);
		Tree->Head = _BalanceTree_(Last->LeftChild);
	}
	else {
		NdCreate(&(Last->RightChild));
		Last->RightChild->Value = Value;
		Last->RightChild->Parent = Last;
		_SetLeftRight_(Last->RightChild, Tree);
		Tree->Head = _BalanceTree_(Last->RightChild);
	}

	Tree->Size++;
	return SUCCESS;
}

int TreeGetCount(CC_TREE* Tree)
{
	if (Tree == NULL)
		return INVALID_PARAMETER;

	return Tree->Size;
}

int TreeContains(CC_TREE* Tree, int Value)
{
	if (Tree == NULL)
		return INVALID_PARAMETER;

	Node* Head = Tree->Head;

	while (Head != NULL) {
		
		if (Head->Value == Value)
			return 1;
		if (Head->Value > Value)
			Head = Head->RightChild;
		else
			Head = Head->LeftChild;
	}

	return 0;
}

int TreeGetHeight(CC_TREE* Tree)
{
	if (Tree == NULL)
		return INVALID_PARAMETER;
	
	if (Tree->Head == NULL)
		return 0;

	if (Tree->Head->RightChild == NULL)
		if (Tree->Head->LeftChild == NULL)
			return 0;
		else
			return 1;
	
	if (Tree->Head->LeftHeight > Tree->Head->RightHeight)
		return Tree->Head->LeftHeight;
	else
		return Tree->Head->RightHeight;
}

int TreeRemove(CC_TREE* Tree, int Value)
{
	if (Tree == NULL)
		return INVALID_PARAMETER;

	Node* Head = Tree->Head;

	Node* ExistentValue = _GetNode_(Tree, Value);

	if (ExistentValue != NULL && ExistentValue->Count > 1) {
		Tree->Size--;
		ExistentValue->Count--;
		return SUCCESS;
	}

	while (Head != NULL) {

		if (Head->Value == Value)
			break;
		if (Head->Value < Value)
			Head = Head->RightChild;
		else
			Head = Head->LeftChild;
	}

	if (Head == NULL)
		return EXEC_FAILURE;
	
	
	Tree->Size--;
	if (_IsLeaf_(Head)) 
		_DeleteLeaf_(Head, Tree);
	else if ( _Has2Sons_(Head) ) {
		_DeleteWith2Sons_(Head, Tree);
	}
	else
		_DeleteWith1Son_(Head, Tree);
		
	
	return SUCCESS;
}

int TreeClear(CC_TREE* Tree)
{
	if (Tree == NULL)
		return INVALID_PARAMETER;

	Node* Head = Tree->Head;

	while (Head != NULL) {

		if (Head->LeftChild != NULL) {
			Head = Head->LeftChild;
			continue;
		}
		if (Head->RightChild != NULL) {
			Head = Head->RightChild;
			continue;
		}
		Node* Parent = Head->Parent;

		if (Parent != NULL) {
			if (Head->Value < Parent->Value)
				Parent->LeftChild = NULL;

			else
				Parent->RightChild = NULL;
		}

		free(Head);
		Head = Parent;
	}

	Tree->Size = 0;
	Tree->Head = NULL;
	return SUCCESS;
}

int TreeGetNthPreorder(CC_TREE* Tree, int Index, int* Value)
{
	if (Tree == NULL || Index < 0 || Index > Tree->Size - 1)
		return INVALID_PARAMETER;

	int Current = -1;
	CC_STACK* s = NULL;
	StCreate(&s);

	if (Tree->Head != NULL) {
		StPush(s, Tree->Head);
	}

	while (StIsEmpty(s) != CC_TRUE) {
		
		Node* node = NULL;
		StPop(s, &node);
		Current++;
		if (Current == Index) {
			*Value = node->Value;
			StDestroy(&s);
			return SUCCESS;
		}
		
		while (node->Count != 1) {
			Current++;
			node->Count--;
			if (Current == Index) {
				*Value = node->Value;
				StDestroy(&s);
				return SUCCESS;
			}

		}
		Node* LeftChild = node->LeftChild, *RightChild = node->RightChild;
		if (LeftChild != NULL)
			StPush(s, RightChild);
		if (RightChild != NULL)
			StPush(s, LeftChild);
	}

	StDestroy(&s);
	return SUCCESS;

}

int TreeGetNthInorder(CC_TREE* Tree, int Index, int* Value)
{
	if (Tree == NULL || Index < 0 || Index > Tree->Size - 1)
		return INVALID_PARAMETER;

	Node* Head = Tree->Head;

	CC_STACK* s = NULL;
	StCreate(&s);
	int Current = -1;

	while (StIsEmpty(s) != CC_TRUE || Head != NULL) {

		while (Head != NULL) {
			StPush(s, Head);
			Head = Head->LeftChild;
		}
	
		StPop(s, &Head);
		Current++;
		if (Current == Index) {
			*Value = Head->Value;
			StDestroy(&s);
			return SUCCESS;
		}
	
		while (Head->Count != 1) {
			Current++;
			Head->Count--;
			if (Current == Index) {
				*Value = Head->Value;
				StDestroy(&s);
				return SUCCESS;
			}

		}
		Head = Head->RightChild;
	}
	StDestroy(&s);
	return SUCCESS;
}

int TreeGetNthPostorder(CC_TREE* Tree, int Index, int* Value)
{
	if (Tree == NULL || Index < 0 || Index > Tree->Size - 1)
		return INVALID_PARAMETER;

	CC_STACK* s = NULL;
	StCreate(&s);

	Node* Head = Tree->Head;
	int Current = -1;

	CC_VECTOR* visited = NULL;
	VecCreate(&visited);


	while (StIsEmpty(s) != CC_TRUE || Head != NULL) {
		
		while (Head != NULL) {
			
			StPush(s, Head);
			VecInsertTail(visited, 0);
			Head = Head->LeftChild;
		}
		StPop(s, &Head);
		
		int value;
		int index = VecGetCount(visited) - 1;
		VecGetValueByIndex(visited, index , &value);
		VecRemoveByIndex(visited, index);
		
		if (value == 0) {
			VecInsertTail(visited, value + 1);
			StPush(s, Head);
			Head = Head->RightChild;
		}
		else {
			Current++;
			if (Current == Index) {
				*Value = Head->Value;
				VecDestroy(&visited);
				StDestroy(&s);
				return SUCCESS;
			}
			while (Head->Count != 1) {
				Current++;
				Head->Count--;
				if (Current == Index) {
					*Value = Head->Value;
					VecDestroy(&visited);
					StDestroy(&s);
					return SUCCESS;
				}

			}
			Head = NULL;
		}
		
	}
	VecDestroy(&visited);
	StDestroy(&s);
	return SUCCESS;
}

void srd(Node* Head) {
	if (!Head)return;

	srd(Head->LeftChild);

	printf(" %d(%d) ", Head->Value,Head->Count);

	srd(Head->RightChild);
}

