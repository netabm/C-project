#include <stdio.h>
#include <stdlib.h>
#include "list.h"

BOOL L_init(LIST* pList)
{
	if (pList == NULL) return False;	// no list to initialize

	pList->head.next = NULL;
	return True;
}

NODE* L_insert(NODE* pNode, DATA Value)
{
	NODE* tmp;

	if (!pNode) return NULL;

	tmp = (NODE*)malloc(sizeof(NODE));	// new node

	if (tmp != NULL) {
		tmp->key = Value;
		tmp->next = pNode->next;
		pNode->next = tmp;
	}
	return tmp;
}

BOOL L_delete(NODE* pNode, void(*freeKey)(void*))
{
	NODE* tmp;

	if (!pNode || !(tmp = pNode->next)) return False;

	pNode->next = tmp->next;
	if (freeKey)
		freeKey(tmp->key);
	free(tmp);
	return True;
}

NODE* L_find(NODE* pNode, DATA Value, int(*compare)(const void*, const void*))
{
	NODE* temp = NULL;
	while (pNode != NULL)
	{
		if (compare(pNode->key, Value) == 0)
		{
			temp = pNode;
			break;
		}
		pNode = pNode->next;
	}

	return temp;


}

BOOL L_free(LIST* pList, void(*freeKey)(void*))
{
	NODE* tmp;

	if (!pList) return False;

	for (tmp = &(pList->head); L_delete(tmp, freeKey); );
	return True;
}

int L_print(LIST* pList, void(*print)(const void*))
{
	NODE* tmp;
	int		c = 0;

	if (!pList) return 0;

	printf("\n");
	for (tmp = pList->head.next; tmp; tmp = tmp->next, c++)
		print(tmp->key);
	printf("\n");
	return c;
}
