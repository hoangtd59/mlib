#ifndef _AVL_H_
#define _AVL_H_

#include "stdafx.h"
//#include <stdio.h>

typedef struct {
	int bf;                            // balance factor  
	void* val;
	struct NODE* pLeft;
	struct NODE* pRight;
} NODE;

typedef struct {
	struct NODE* pRoot;
	size_t size;
	void* (*mkval)(void*);             // funtion that make value of node
	void(*outval)(void*, FILE*);       // funtion that output value of node to stream
	void(*freeval)(void*);             // funtion that free value of node
	int(*cmpval)(void*, void*);        // funtion that compare value of two node
} AVL;

void avl_init(AVL* tree,
	void* (*mkval)(void*),
	void(*outval)(void*, FILE*),
	void(*freeval)(void*),
	int(*cmpval)(void*, void*));

// direct : "LNR", "NLR", "LRN"
void avl_out(const AVL* tree, FILE* odev, char* direct);
void avl_dosth(AVL* tree, char* direct, void* item, void (*work)(void*, void*));
size_t avl_size(const AVL* tree);
int avl_empty(const AVL* tree);
void* avl_insert(AVL* tree, const void* val);
void* avl_find(AVL* tree, const void* val);
void avl_erase(AVL* tree, const void* val);
void avl_clear(AVL* tree);
void avl_free(AVL** tree);


#endif