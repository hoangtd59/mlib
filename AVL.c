#include "stdafx.h"

#include "avl.h"
//#include <stdlib.h>
//#include <string.h>

// Static funtion /////////////////////////////////////
static void* _val_return = NULL;

static enum BALANCE_FACTOR {
	LHT = -1, // left higher than
	EQH = 0, // equal high
	RHT = 1 // right higher than
};

static inline void* mkvaldef(void* val)
{
	return val;
}

static inline void outvaldef(void* val, FILE* odev) {}

static inline void freevaldef(void* val)
{
	free(val);
}

static inline int cmpvaldef(void* l_val, void* r_val)
{
	return 0;
}

static void swap(void** lval, void** rval)
{
	void* t = *lval;
	*lval = *rval;
	*rval = *lval;
}

static NODE* mknode(void* val, void* (*mkval)(void*))
{
	NODE* pn = malloc(sizeof(NODE));
	if (pn) {
		pn->bf = EQH;
		pn->val = mkval(val);
		pn->pLeft = pn->pRight = NULL;
	}
	return pn;
}

static void freenode(NODE** pNode, void (*freeval)(void* val))
{
	freeval((*pNode)->val);
	free((*pNode));
	(*pNode) = NULL;
}

static void avl_out_lnr_imp(const NODE* pRoot, FILE* odev, void(*outval)(void*, FILE*))
{
	if (pRoot == NULL)
		return;
	avl_out_lnr_imp(pRoot->pLeft, odev, outval);
	outval(pRoot->val, odev);
	avl_out_lnr_imp(pRoot->pRight, odev, outval);
}

static void avl_out_nlr_imp(const NODE* pRoot, FILE* odev, void(*outval)(void*, FILE*))
{
	if (pRoot == NULL)
		return;
	outval(pRoot->val, odev);
	avl_out_nlr_imp(pRoot->pLeft, odev, outval);
	avl_out_nlr_imp(pRoot->pRight, odev, outval);
}

static void avl_out_lrn_imp(const NODE* pRoot, FILE* odev, void(*outval)(void*, FILE*))
{
	if (pRoot == NULL)
		return;
	avl_out_lrn_imp(pRoot->pLeft, odev, outval);
	avl_out_lrn_imp(pRoot->pRight, odev, outval);
	outval(pRoot->val, odev);
}

//////////
static void avl_dosth_lnr_imp(NODE* pRoot, void* item, void (*work)(void*, void*))
{
	if (pRoot == NULL)
		return;
	avl_dosth_lnr_imp(pRoot->pLeft, item, work);
	work(pRoot->val, item);
	avl_dosth_lnr_imp(pRoot->pRight, item, work);
}

static void avl_dosth_nlr_imp(NODE* pRoot, void* item, void (*work)(void*, void*))
{
	if (pRoot == NULL)
		return;
	work(pRoot->val, item);
	avl_dosth_nlr_imp(pRoot->pLeft, item, work);
	avl_dosth_nlr_imp(pRoot->pRight, item, work);
}

static void avl_dosth_lrn_imp(NODE* pRoot, void* item, void (*work)(void*, void*))
{
	if (pRoot == NULL)
		return;
	avl_dosth_lrn_imp(pRoot->pLeft, item, work);
	avl_dosth_lrn_imp(pRoot->pRight, item, work);
	work(pRoot->val, item);
}

////////////

static void rotateLL(NODE** pRoot)
{
	// check input
	if (*pRoot == NULL || (*pRoot)->pLeft == NULL)
		return;

	// Rotate left
	NODE* pT = *pRoot;
	NODE* pT1 = pT->pLeft;
	pT->pLeft = pT1->pRight;
	pT1->pRight = pT;

	// Refresh balance factor
	switch (pT1->bf) 
	{
		case LHT :
			pT->bf = EQH;
			pT1->bf = EQH;
			break;
		case EQH :
			pT->bf = LHT;
			pT1->bf = RHT;
			break;
	}

	// Refresh pRoot
	*pRoot = pT1;
}

static void rotateRR(NODE** pRoot)
{
	// Check input
	if (*pRoot == NULL || (*pRoot)->pRight == NULL)
		return;

	// Rotate right
	NODE* pT = *pRoot;
	NODE* pT1 = pT->pRight;

	pT->pRight = pT1->pLeft;
	pT1->pLeft = pT;
	switch (pT1->bf)
	{
		case RHT :
			pT->bf = EQH;
			pT1->bf = EQH;
			break;
		case EQH :
			pT->bf = RHT;
			pT1->bf = LHT;
			break;
	}

	// Refresh balance factor
	*pRoot = pT1;
}

static void rotateLR(NODE** pRoot)
{
	// Check input
	if (*pRoot == NULL || (*pRoot)->pLeft == NULL)
		return;

	// Rotate left - right
	NODE* pT = *pRoot;
	NODE* pT1 = pT->pLeft;
	NODE* pT2 = pT1->pRight;

	pT->pLeft = pT2->pRight;
	pT1->pRight = pT2->pLeft;
	pT2->pLeft = pT1;
	pT2->pRight = pT;

	// Refresh balance factor
	switch (pT2->bf)
	{
		case RHT :
			pT->bf = EQH;
			pT1->bf = LHT;
			break;
		case LHT :
			pT->bf = RHT;
			pT1->bf = EQH;
		case EQH :
			pT->bf = pT1->bf = EQH;
	}

	// Refresh pRoot
	*pRoot = pT2;
}

static void rotateRL(NODE** pRoot)
{
	// Check input 
	if (*pRoot == NULL || (*pRoot)->pRight == NULL)
		return;

	// Rotate right - left
	NODE* pT = *pRoot;
	NODE* pT1 = pT->pRight;
	NODE* pT2 = pT1->pLeft;

	pT->pRight = pT2->pLeft;
	pT1->pLeft = pT2->pRight;
	pT2->pLeft = pT;
	pT2->pRight = pT1;

	// Refresh balance factor
	switch (pT2->bf)
	{
		case LHT :
			pT->bf = EQH;
			pT1->bf = RHT;
			break;
		case RHT :
			pT->bf = LHT;
			pT1->bf = EQH;
			break;
		case EQH :
			pT->bf = pT1->bf = EQH;
			break;
	}

	// Refresh pRoot
	*pRoot = pT2;
}

// Return :
//           0 : no rotate (invalid argument)
//           1 : rotate successfully, dont't change high
//           2 : rotae successfully, decrease high
static int balance_left(NODE** pRoot)
{
	// Check input
	if (*pRoot == NULL || (*pRoot)->pLeft == NULL)
		return 0;

	// Balance left 
	NODE* pT = (*pRoot)->pLeft;
	switch (pT->bf) 
	{
		case LHT : // Mất cân bằng trái, lệch trái
			rotateLL(pRoot);
			return 2;
		case EQH : // Mất cân băng trái, không lệch
			rotateLL(pRoot);
			return 1;
		case RHT : // Mất cân bằng trái, lệch phải
			rotateLR(pRoot);
			return 2;
	}
}

// Return :
//           0 : no rotate (invalid argument)
//           1 : rotate successfully, dont't change high
//           2 : rotae successfully, decrease high
static int balance_right(NODE** pRoot)
{
	// Check input
	if (*pRoot == NULL || (*pRoot)->pRight == NULL)
		return 0;

	// Balance left 
	NODE* pT = (*pRoot)->pRight;
	switch (pT->bf)
	{
	case LHT: // Mất cân bằng phải, lệch trái
		rotateRL(pRoot);
		return 2;
	case EQH: // Mất cân bằng phải, không lệch
		rotateRR(pRoot);
		return 1;
	case RHT: // Mất cân bằng phải, lệch phải
		rotateRR(pRoot);
		return 2;
	}
}

// Return :
//     -1 : fail, couldn't allocate memory
//      0 : the value had existed in the tree
//      1 : successfull, don't change high
//      2 : successfull, increase high
static int avl_insert_imp(NODE** pRoot, const void* val, void* (*mkval)(void*), int(*cmpval)(void*, void*))
{
	int res;
	if (*pRoot == NULL)
	{
		NODE* pN = mknode(val, mkval);
		if (pN == NULL)
			return -1;
		*pRoot = pN;
		_val_return = pN->val;
		return 2;
	}
	if ((cmpval((*pRoot)->val, val) > 0))
	{
		res =  avl_insert_imp(&((*pRoot)->pLeft), val, mkval, cmpval);
		if (res < 2)
			return res;
		switch ((*pRoot)->bf)
		{
			case LHT :
				balance_left(pRoot);
				(*pRoot)->bf = EQH;
				return 1;
			case EQH :
				(*pRoot)->bf = LHT;
				return 2;
			case RHT :
				(*pRoot)->bf = EQH;
				return 1;
		}
	}
	else if ((cmpval((*pRoot)->val, val) < 0))
	{
		res =  avl_insert_imp(&((*pRoot)->pRight), val, mkval, cmpval);
		if (res < 2)
			return res;
		switch ((*pRoot)->bf)
		{
			case LHT :
				(*pRoot)->bf = EQH;
				return 1;
			case EQH :
				(*pRoot)->bf = RHT;
				return 2;
			case RHT :
				balance_right(pRoot);
				return 1;
		}
	}
	return 0;
}

static void* avl_find_imp(NODE* pRoot, const void* val, int(*cmpval)(void*, void*))
{
	if (pRoot == NULL)
		return NULL;
	if (cmpval(pRoot->val, val) == 0)
		return pRoot->val;
	if (cmpval(pRoot->val, val) > 0)
		return avl_find_imp(pRoot->pLeft, val, cmpval);
	else
		return avl_find_imp(pRoot->pRight, val, cmpval);
}

static void avl_free_imp(NODE** pRoot, void(*freeval)(void* val))
{
	if ((*pRoot) == NULL)
		return;
	avl_free_imp(&((*pRoot)->pLeft), freeval);
	avl_free_imp(&((*pRoot)->pRight), freeval);
	freenode(pRoot, freeval);
}

static int search_stand_for(NODE** pRoot, NODE** pStand)
{
	int res;
	if ((*pStand)->pRight)
	{
		res = search_stand_for(pRoot, &(*pStand)->pRight);
		if (res < 2)
			return res;
		switch ((*pStand)->bf)
		{
			case LHT :
				return balance_left(pStand);
			case EQH :
				return 1;
			case RHT :
				return 2;
		}
	}
	swap(&(*pRoot)->val, &(*pStand)->val);
	(*pRoot) = (*pStand);
	(*pStand) = (*pStand)->pLeft;
	return 2;
}

// Return :
//      0 : val wasn't found
//      1 : erase successfully
static int avl_erase_imp(NODE** pRoot, const void* val, int(*cmpval)(void*, void*), void (*freeval)(void*))
{
	if ((*pRoot) == NULL)
		return 0;
	int res;
	if (cmpval((*pRoot)->val, val) == 0)
	{
		NODE* pT = *pRoot;
		if (pT->pLeft == NULL)
			(*pRoot) = (*pRoot)->pRight;
		else if (pT->pRight == NULL)
			(*pRoot) = (*pRoot)->pLeft;
		else
		{
			res = search_stand_for(&pT, &(pT->pLeft));
			if (res < 2)
				return res;
			switch (pT->bf)
			{
				case LHT:
					pT->bf = EQH;
					return 2;
				case EQH:
					pT->bf = RHT;
					return 1;
				case RHT:
					return balance_right(pRoot);
			}
		}
		freenode(&pT, freeval);
	}
	else
	{
		if (cmpval((*pRoot)->val, val) > 0)
		{
			res = avl_erase_imp(&((*pRoot)->pLeft), val, cmpval, freeval);
			if (res < 2)
				return res;
			switch ((*pRoot)->bf)
			{
				case LHT :
					(*pRoot)->bf = EQH;
					return 2;
				case EQH :
					(*pRoot)->bf = RHT;
					return 1;
				case RHT :
					return balance_right(pRoot);
			}
		}
		else // (cmpval((*pRoot)->val, val) < 0)
		{
			res = avl_erase_imp((&(*pRoot)->pRight), val, cmpval, freeval);
			if (res < 2)
				return res;
			switch ((*pRoot)->bf)
			{
				case LHT :
					return balance_left(pRoot);
				case EQH :
					(*pRoot)->bf = LHT;
					return 1;
				case RHT :
					(*pRoot)->bf = EQH;
					return 2;
			}
		}
	}
}

// End static funtion ////////////////////////////////

// External funtion /////////////////////////////////
void avl_init(AVL* tree, void* (*mkval)(void*), void(*outval)(void*, FILE*), void(*freeval)(void*), int(*cmpval)(void*, void*))
{
	tree->pRoot = NULL;
	tree->size = 0;
	tree->mkval = mkval == NULL ? mkvaldef : mkval;
	tree->outval = outval == NULL ? outvaldef : outval;
	tree->freeval = freeval == NULL ? freevaldef : freeval;
	tree->cmpval = cmpval == NULL ? cmpvaldef : cmpval;
}

void avl_out(const AVL* tree, FILE* odev, char* direct)
{
	if (tree && odev)
	{
		if (direct == NULL || strcmp(direct, "LNR") == 0)
			avl_out_lnr_imp(tree->pRoot, odev, tree->outval);
		else if (strcmp(direct, "NLR") == 0)
			avl_out_nlr_imp(tree->pRoot, odev, tree->outval);
		else if (strcmp(direct, "LRN") == 0)
			avl_out_lrn_imp(tree->pRoot, odev, tree->outval);
		else fprintf(stderr, "in funtion avl_out() : wrong 3rd argument !\n");
	}
}

void avl_dosth(AVL* tree, char* direct, void* item, void (*work)(void*, void*))
{
	if (tree && work)
	{
		if (direct == NULL || strcmp(direct, "LNR") == 0)
			avl_dosth_lnr_imp(tree->pRoot, item, work);
		else if (strcmp(direct, "NLR") == 0)
			avl_dosth_nlr_imp(tree->pRoot, item, work);
		else if (strcmp(direct, "LRN") == 0)
			avl_dosth_lrn_imp(tree->pRoot, item, work);
		else fprintf(stderr, "in funtion avl_out() : wrong 3rd argument !\n");
	}
}


size_t avl_size(const AVL* tree)
{
	return tree->size;
}

int avl_empty(const AVL* tree)
{
	return (tree == NULL || tree->size == 0);
}

void* avl_insert(AVL* tree, const void* val)
{
	_val_return = NULL;
	if (avl_insert_imp(&(tree->pRoot), val, tree->mkval, tree->cmpval) > 0)
		tree->size++;
	return _val_return;
}

void* avl_find(AVL* tree, const void* val)
{
	return avl_find_imp(tree->pRoot, val, tree->cmpval);
}

void avl_erase(AVL* tree, const void* val)
{
	if (avl_erase_imp(&(tree->pRoot), val, tree->cmpval, tree->freeval))
		tree->size--;
}

void avl_clear(AVL* tree)
{
	avl_free_imp(&(tree)->pRoot, tree->freeval);
	tree->size = 0;
}

void avl_free(AVL** tree)
{
	avl_free_imp(&((*tree)->pRoot), (*tree)->freeval);
	free((*tree));
	(*tree) = NULL;
}

// End external funtion /////////////////////////////////