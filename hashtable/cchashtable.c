#include "cchashtable.h"

int _HashCode(char* Key)
{
	if (Key == NULL)
		return INVALID_PARAMETER;

	int Code = Key[0];

	return (Code % 26);
}

int _StrCmp(char* First, char* Second)
{
	if (First == NULL)
		return INVALID_PARAMETER;

	if (Second == NULL)
		return INVALID_PARAMETER;

	int i = 0;
	int flag = 0;
	while (flag == 0)
	{
		if (First[i] > Second[i])
			return 1;
		else if (First[i] < Second[i])
			return -1;
		if (First[i] == '\0')
			break;
		i++;
	}
	return flag;
}

// Allocates the memory necessary for the Destination
int _MemCpy(char** Destination, char* Source)
{
	if (Source == NULL)
		return INVALID_PARAMETER;

	if ((*Destination) != NULL)
		return INCORRECT_PARAMETER;

	int SrcLength = 0;
	int i = 0;
	while (Source[i] != '\0')
	{
		SrcLength++;
		i++;
	}

	(*Destination) = (char*)calloc(SrcLength + 1, sizeof(char));

	if ((*Destination) == NULL)
		return MALLOC_FAILURE;

	for (i = 0; i < SrcLength; i++)
		(*Destination)[i] = Source[i];
	(*Destination)[SrcLength] = '\0';

	return SUCCESS;
}

int _HashListResize(HASH_LIST** Hl, int newCapacity)
{
	if ((*Hl) == NULL)
		return INVALID_PARAMETER;

	char** newKeys = (char**)malloc(sizeof(char*) * newCapacity);

	if (newKeys == NULL)
		return MALLOC_FAILURE;

	int* newValues = (int*)malloc(sizeof(int) * newCapacity);
	if (newValues == NULL) {
		free(newKeys);
		return MALLOC_FAILURE;
	}

	for (int i = 0; i < (*Hl)->Size; i++) {
		newKeys[i] = (*Hl)->Keys[i];
		newValues[i] = (*Hl)->Values[i];
	}

	free((*Hl)->Values);
	free((*Hl)->Keys);

	(*Hl)->Keys = newKeys;
	(*Hl)->Values = newValues;
	(*Hl)->Capacity = newCapacity;

	return SUCCESS;
}

int HtPrint(CC_HASH_TABLE* HashTable)
{
	if (HashTable == NULL)
		return INVALID_PARAMETER;

	for (int i = 0; i < HashTable->Size; i++)
	{
		printf("%d: ", i);
		for (int j = 0; j < HashTable->Lists[i]->Size; j++) {
			printf("<%s", HashTable->Lists[i]->Keys[j]);
			printf(", %d> ", HashTable->Lists[i]->Values[j]);
		}
		printf("\n");
	}

	return SUCCESS;
}

int HtCreate(CC_HASH_TABLE** HashTable)
{
	if (HashTable == NULL)
		return INVALID_PARAMETER;

	if ((*HashTable) != NULL)
		return INVALID_PARAMETER;

	(*HashTable) = (CC_HASH_TABLE*)malloc(sizeof(CC_HASH_TABLE));

	if ((*HashTable) == NULL)
		return MALLOC_FAILURE;

	(*HashTable)->Size = HT_SIZE;
	(*HashTable)->Lists = (HASH_LIST**)malloc(sizeof(HASH_LIST*) * (*HashTable)->Size);

	if ((*HashTable)->Lists == NULL)
		return MALLOC_FAILURE;

	int i;
	for (i = 0; i < (*HashTable)->Size; i++)
	{
		(*HashTable)->Lists[i] = (HASH_LIST*)malloc(sizeof(HASH_LIST));

		if ((*HashTable)->Lists[i] == NULL)
			return MALLOC_FAILURE;
		
		(*HashTable)->Lists[i]->Size = 0;
		(*HashTable)->Lists[i]->Capacity = 10;

		(*HashTable)->Lists[i]->Keys = (char**)malloc(sizeof(char*) * (*HashTable)->Lists[i]->Capacity);
		(*HashTable)->Lists[i]->Values = (int*)malloc(sizeof(int) * (*HashTable)->Lists[i]->Capacity);

		if ((*HashTable)->Lists[i]->Keys == NULL)
			return MALLOC_FAILURE;

		if ((*HashTable)->Lists[i]->Values == NULL)
			return MALLOC_FAILURE;
		
		for (int j = 0; j < (*HashTable)->Lists[i]->Capacity; j++) {
			(*HashTable)->Lists[i]->Keys[j] = NULL;
			(*HashTable)->Lists[i]->Values[j] = 0;
		}
	}

	return SUCCESS;
}

int HtDestroy(CC_HASH_TABLE** HashTable)
{
	if (HashTable == NULL)
		return INVALID_PARAMETER;

	for (int i = 0; i < (*HashTable)->Size; i++)
	{
		free((*HashTable)->Lists[i]->Values);
		for (int j = 0; j < (*HashTable)->Lists[i]->Size; j++)
			if ((*HashTable)->Lists[i]->Keys[j] != NULL)
				free((*HashTable)->Lists[i]->Keys[j]);
		free((*HashTable)->Lists[i]->Keys);
		free((*HashTable)->Lists[i]);
	}

	free((*HashTable)->Lists);
	free((*HashTable));

	(*HashTable) = NULL;

	return SUCCESS;
}

int HtSetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int Value)
{
	if (HashTable == NULL)
		return INVALID_PARAMETER;

	if (Key == NULL)
		return INVALID_PARAMETER;

	int Exists = 0;
	int i;
	int HashCode = _HashCode(Key);

	for (i = 0; i < HashTable->Lists[HashCode]->Size; i++)
		if (_StrCmp(HashTable->Lists[HashCode]->Keys[i], Key) == 0 ||
			_StrCmp(Key, HashTable->Lists[HashCode]->Keys[i]) == 0) {
			HashTable->Lists[HashCode]->Values[i] = Value;
			Exists = 1;
		}

	if (Exists == 0)
	{
		char* newKey = NULL;
		if (_MemCpy(&newKey, Key) < 0)
			return EXEC_FAILURE;
		if (HashTable->Lists[HashCode]->Size + 1 == HashTable->Lists[HashCode]->Capacity)
			if (_HashListResize(&(HashTable->Lists[HashCode]), 2 * HashTable->Lists[HashCode]->Capacity) < 0)
				return EXEC_FAILURE;
		HashTable->Lists[HashCode]->Keys[HashTable->Lists[HashCode]->Size] = newKey;
		HashTable->Lists[HashCode]->Values[HashTable->Lists[HashCode]->Size] = Value;
		HashTable->Lists[HashCode]->Size++;
	}
	return SUCCESS;
}

int HtGetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int* Value)
{
	if (HashTable == NULL)
		return INVALID_PARAMETER;

	if (Key == NULL)
		return INVALID_PARAMETER;

	if (Value == NULL)
		return INCORRECT_PARAMETER;

	int Code = _HashCode(Key);
	(*Value) = INT_MIN;
	for (int i = 0; i < HashTable->Lists[Code]->Size; i++)
		if (_StrCmp(HashTable->Lists[Code]->Keys[i], Key) == 0 ||
			_StrCmp(Key, HashTable->Lists[Code]->Keys[i]) == 0)
			(*Value) = HashTable->Lists[Code]->Values[i];

	return SUCCESS;
}

int HtRemoveKey(CC_HASH_TABLE* HashTable, char* Key)
{
	if (HashTable == NULL)
		return INVALID_PARAMETER;

	if (Key == NULL)
		return INVALID_PARAMETER;

	int Code = _HashCode(Key);
	int Exists = 0;
	int i = 0;

	while (i < HashTable->Lists[Code]->Size && Exists == 0) {
		if (_StrCmp(HashTable->Lists[Code]->Keys[i], Key) == 0 ||
			_StrCmp(Key, HashTable->Lists[Code]->Keys[i]) == 0)
			Exists = 1;
		else
			i++;
	}

	if (Exists == 0)
		return INCORRECT_PARAMETER;
	else
	{
		char* aux = HashTable->Lists[Code]->Keys[i];
		for (int j = i; j < HashTable->Lists[Code]->Size - 1; j++) {
			HashTable->Lists[Code]->Keys[j] = HashTable->Lists[Code]->Keys[j + 1];
			HashTable->Lists[Code]->Values[j] = HashTable->Lists[Code]->Values[j + 1];
		}
		HashTable->Lists[Code]->Size--;
		free(aux);
	}

	return SUCCESS;
}

int HtHasKey(CC_HASH_TABLE* HashTable, char* Key)
{
	if (HashTable == NULL)
		return INVALID_PARAMETER;

	if (Key == NULL)
		return INVALID_PARAMETER;

	int Code = _HashCode(Key);

	for (int i = 0; i < HashTable->Lists[Code]->Size; i++)
		if (_StrCmp(HashTable->Lists[Code]->Keys[i], Key) == 0 ||
			_StrCmp(Key, HashTable->Lists[Code]->Keys[i]) == 0)
			return SUCCESS;
	return 1;
}

int HtGetNthKey(CC_HASH_TABLE* HashTable, int Index, char** Key)
{
	if (HashTable == NULL)
		return INVALID_PARAMETER;

	if (Key == NULL)
		return INVALID_PARAMETER;

	if ((*Key) != NULL)
		return INVALID_PARAMETER;

	if (Index < 0)
		return INCORRECT_PARAMETER;

	int i = 0;
	int j = 0;
	int index = 0;
	for (i = 0; i < HashTable->Size; i++)
		for (j = 0; j < HashTable->Lists[i]->Size; j++)
		{
			if (index == Index)
				goto Found;
			index++;
		}
	goto NotFound;

Found:
	if (_MemCpy(&(*Key), HashTable->Lists[i]->Keys[j]) < 0)
		return EXEC_FAILURE;
	goto End;

NotFound:
	(*Key) = NULL;

End:
	return SUCCESS;
}

int HtClear(CC_HASH_TABLE* HashTable)
{
	if (HashTable == NULL)
		return INVALID_PARAMETER;

	int i;
	int j;
	for (i = 0; i < HashTable->Size; i++) {
		for (j = 0; j < HashTable->Lists[i]->Size; j++)	{
			free(HashTable->Lists[i]->Keys[j]);
			HashTable->Lists[i]->Keys[j] = NULL;
			HashTable->Lists[i]->Values[j] = 0;
		}
		HashTable->Lists[i]->Size = 0;
	}

	return SUCCESS;
}

int HtGetKeyCount(CC_HASH_TABLE* HashTable)
{
	if (HashTable == NULL)
		return INVALID_PARAMETER;

	int Sum = 0;
	for (int i = 0; i < HashTable->Size; i++)
		Sum += HashTable->Lists[i]->Size;

	return Sum;
}



