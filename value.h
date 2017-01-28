#ifndef _VALUE_H_
#define _VALUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AVL.h"
#include "hash.h"

// hash_t ///////////////////////////////
static void* mkhasht(void* val)
{
	hash_t* res = (hash_t*)malloc(sizeof(hash_t));
	*res = *(hash_t*)val;
	return res;
}

static int cmphasht(void* lval, void* rval)
{
	hash_t l = *(hash_t*)lval;
	hash_t r = *(hash_t*)rval;
	if (l > r)
		return 1;
	if (l == r)
		return 0;
	return -1;
}

// word ///////////////////////////////
static void* mkword(void* val)
{
	char* res = _strdup((char*)val);
	return res;
}

static void outword(void* val, FILE* odev)
{
	fprintf(odev, "%s ", (char*)val);
}

static void outword2(void* val, FILE* odev)
{
	fprintf(odev, "%s\n", (char*)val);
}

static void freeword(void* val)
{
	free(val);
}

static int cmpword(void* lval, void* rval)
{
	return strcmp((char*)lval, (char*)rval);
}

// subject ////////////////////////////
typedef struct {
	char* val;
	AVL* tree_word;
} Subject;

// val : char* : subject content
static void* mksubj(void* val)
{
	Subject* res = (Subject*)malloc(sizeof(Subject));
	res->val = _strdup((char*)val);
	res->tree_word = (AVL*)malloc(sizeof(AVL));
	avl_init(res->tree_word, mkword, outword, freeword, cmpword);
	return res;
}

static void outsubj(void* val, FILE* odev)
{
	Subject* sub = (Subject*)val;
	fprintf(odev, "#%s %u\n", sub->val, avl_size(sub->tree_word));
	avl_out(sub->tree_word, odev, NULL);
	fprintf(odev, "\n\n");
}

static void freesubj(void* val)
{
	Subject* sub = (Subject*)val;
	free(sub->val);
	avl_free(&(sub->tree_word));
	free(sub);
}

// rval : char* : subject content
static int cmpsubj(void* lval, void* rval)
{
	Subject* lsub = (Subject*)lval;
	char* rstr = (char*)rval;
	return strcmp(lsub->val, rstr);
}


#endif