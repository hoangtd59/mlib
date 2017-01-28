//#include <stdlib.h>
#include "stdafx.h"

#include "uqueue.h"


/////////////////////////////////////
static void* mkvaldef(void* val)
{
	return val;
}

static void outvaldef(void* val, FILE* odev)
{
	//...
}

static void freevaldef(void* val)
{
	free(val);
}

static int cmpvaldef(void* lval, void* rval)
{
	return 1;
}

static UQnode* mknode(void* val, void* (*mkval)(void*))
{
	UQnode* pnode = (UQnode*)malloc(sizeof(UQnode));
	pnode->pNext = NULL;
	pnode->val = mkval(val);
	return pnode;
}

static void freenode(UQnode** pnode, void (*freeval)(void*))
{
	if (*pnode)
	{
		freeval((*pnode)->val);
		free(*pnode);
		*pnode = NULL;
	}
}

/////////////////////////////////////

void uqueue_init(uqueue* UQ, void* (*mkval)(void*), void(*outval)(void*, FILE*), void(*freeval)(void*), int(*cmpval)(void*, void*))
{
	UQ->pHead = UQ->pTail = NULL;
	UQ->mkval = mkval ? mkval : mkvaldef;
	UQ->outval = outval ? outval : outvaldef;
	UQ->freeval = freeval ? freeval : freevaldef;
	UQ->cmpval = cmpval ? cmpval : cmpvaldef;
}

int uqueue_push(uqueue* UQ, void* val)
{
	if (UQ && val)
	{
		// find val in queue
		UQnode* pT = UQ->pHead;
		while (pT)
		{
			if (UQ->cmpval(pT->val, val) == 0)
				return 2;
			pT = pT->pNext;
		}
		// make UQnode
		UQnode* pnode = mknode(val, UQ->mkval);
		if (pnode == NULL)
		{
			perror("uqueue_push() : ");
			return 0;
		}
		// queue empty
		if (UQ->pHead == NULL)
			UQ->pHead = UQ->pTail = pnode;
		else
		{
			UQ->pTail->pNext = pnode;
			UQ->pTail = pnode;
		}
		return 1;
	}
	fprintf(stderr, "uqueue_push() : invalid argument.\n");
	return 0;
}

void uqueue_pop(uqueue* UQ)
{
	if (UQ)
	{
		UQnode* ptemp = UQ->pHead;
		UQ->pHead = UQ->pHead->pNext;
		freenode(&ptemp, UQ->freeval);
	}
}

void* uqueue_first(uqueue* UQ)
{
	if (UQ)
	{
		return UQ->pHead;
	}
	return NULL;
}

void uqueue_close(uqueue* UQ)
{
	if (UQ)
	{
		UQnode* pT;
		while (UQ->pHead)
		{
			pT = UQ->pHead;
			UQ->pHead = UQ->pHead->pNext;
			freenode(&pT, UQ->freeval);
		}
		free(UQ);
	}
}

int uqueue_empty(uqueue* UQ)
{
	return (UQ == NULL || UQ->pHead == NULL);
}

size_t uqueue_size(uqueue* UQ)
{
	size_t res = 0;
	if (UQ)
	{
		UQnode* pT = UQ->pHead;
		while (pT)
		{
			res++;
			pT = pT->pNext;
		}
	}
	return res;
}

void uqueue_out(uqueue* UQ, FILE* odev)
{
	if (UQ)
	{
		UQnode* pT = UQ->pHead;
		while (pT)
		{
			UQ->outval(pT->val, odev);
			pT = pT->pNext;
		}
	}
}

void uqueue_dosth(uqueue* UQ, void* rval, void(*work)(void*, void*))
{
	if (UQ)
	{
		UQnode* pT = UQ->pHead;
		while (pT)
		{
			work(pT->val, rval);
			pT = pT->pNext;
		}
	}
}


/////////////////////////