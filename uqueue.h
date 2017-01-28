#ifndef _UQUEUE_H_
#define _UQUEUE_H_

#include "stdafx.h"
//#include <stdio.h>

typedef struct {
	void* val;
	struct UQnode* pNext;
} UQnode;

typedef struct {
	UQnode* pHead;
	UQnode* pTail;
	void* (*mkval)(void*);
	void(*outval)(void*, FILE*);
	void(*freeval)(void*);
	int(*cmpval)(void*, void*);
} uqueue;

void uqueue_init(uqueue* UQ, 
	void* (*mkval)(void*), 
	void(*outval)(void*, FILE*),
	void(*freeval)(void*), 
	int (*cmpval)(void*, void*));

int uqueue_push(uqueue* UQ, void* val);

void uqueue_pop(uqueue* UQ);

void* uqueue_first(uqueue* UQ);

void uqueue_close(uqueue* UQ);

int uqueue_empty(uqueue* UQ);

size_t uqueue_size(uqueue* UQ);

void uqueue_out(uqueue* UQ, FILE* odev);

void uqueue_dosth(uqueue* UQ, void* rval, void(*work)(void*, void*));

#endif




