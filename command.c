#include "stdafx.h"

#include "command.h"

// Static function ////////////////////////////////
int InHashTable(char** hashTable)
{
	long wcount = 0, hcount = 0; //d
	delDelim(PATH_LIB);
	FILE* idev = fopen(PATH_TEMPT, "rt");
	if (idev == NULL)
	{
		perror("InHashT() : ");
		return false;
	}
	char s[MAX_WORD_SIZE];
	char* stempt = NULL;
	hash_t hcode;
	while (feof(idev) == 0 && fscanf(idev, "%s", s) > 0)
	{
		wcount++; //d
		stempt = s;
		if (s[0] == '#')
			stempt = (char*)s + 1;
		hcode = hash_str(stempt, MODULE);
		if (hashTable[hcode] == NULL)
		{
			hcount++; //d
			hashTable[hcode] = _strdup(stempt);
		}
		//else
		//{
		//	hash_t hcode_sec = hash_str_sec(stempt, MODULE, MAXR);
		//	for (size_t i = 0; i < MAX_WORD; i++)
		//	{
		//		hcode = hcode + i * hcode_sec;
		//		if (hashTable[hcode] == NULL)
		//		{
		//			hcount++; //d
		//			hashTable[hcode] = _strdup(stempt);
		//			break;
		//		}
		//	}
		//}
	}
	printf("word count : %ld\nhash count : %ld\n", wcount, hcount); //d
	fclose(idev);
	remove(PATH_TEMPT);
	return true;
}

int findWord(char** hashTable, char* s)
{
	int res = false;
	char dest[MAX_WORD_SIZE] = "";
	normalWord(s);
	hash_t hcode = hash_str(s, MODULE);

	if (hashTable[hcode] != NULL)
	{
		if (strcmp(s, hashTable[hcode]) == 0)
			return true;
		else
			if (!(res = delSES(dest, s)) || (res && (hashTable[hcode] == NULL)))
				if (!(res = delING(dest, s)) || (res && (hashTable[hcode] == NULL)))
					if (!(res = delED(dest, s)) || (res && (hashTable[hcode] == NULL)))
						return false;
		return true;
	}
	return false;
}

void freeHashT(char** hashTable)
{
	size_t i;
	for (i = 0; i < MODULE; i++)
		free(hashTable[i]);
	free(hashTable);
}

int InTreeSubj(AVL* tree_subj)
{
	// open file PATH_LIB
	FILE* idev = fopen(PATH_LIB, "rt");
	if (idev == NULL)
	{
		perror("InTreeSubj() : ");
		return false;
	}

//	char sbuf[MAX_WORD_SIZE] = { 0 };
	char sbuf[MAX_WORD_SIZE];
	char* sbuf_data = NULL;
	char temp[40] = "";
	if (sbuf == NULL)
		return;
	void* pbuf = NULL;
	Subject* sub = NULL;
	AVL* tree_word = NULL;
	size_t count;

	// input subject
	if (fscanf(idev, "%[^#]%s", temp, sbuf) > 0 && sbuf[0] == '#') 
	{
		do {
			// input count
			fscanf(idev, "%u", &count);     
			sbuf_data = (char*)sbuf + 1;
			normalWord(sbuf_data);
			pbuf = avl_find(tree_subj, sbuf_data);
			// if subject wasn't found in library
			if (pbuf == NULL)
				// insert subject to library
				pbuf = avl_insert(tree_subj, sbuf_data);
			if (pbuf != NULL)
			{
				sub = (Subject*)pbuf;
				tree_word = sub->tree_word;  
				// input word
				while (feof(idev) == 0 && fscanf(idev, "%s", sbuf) > 0 && sbuf[0] != '#') 
				{
					// find word
					normalWord(sbuf);
					pbuf = avl_find(tree_word, sbuf); 
					// if word wasn't found in subject tree
					if (pbuf == NULL)  
						// insert word to subject tree
						avl_insert(tree_word, sbuf); 
				}
			}
		} while (feof(idev) == 0);
	}
	// close file 
	fclose(idev);
	// return and exit
	return true;

}

// function was used in function dosth(), that
// count amount of word of a subject
static void countword(void* subj, void* count)
{
	Subject* temp = (Subject*)subj;
	*(size_t*)count += avl_size(temp->tree_word);
}

int OutTreeSubj(AVL* tree_subj)
{
	// Open file PATH_LIB
	FILE* odev = fopen(PATH_LIB, "wt");
	if (odev == NULL)
	{
		perror("OutTreeSubj() : ");
		return false;
	}
	// data
	AVL* tree_word;
	size_t sub_count = 0;
	size_t word_count = 0;

	// count subject
	sub_count = avl_size(tree_subj);

	// count word
	avl_dosth(tree_subj, NULL, &word_count, countword);

	// output
	fprintf(odev, "subject : %u         word : %u\n\n", sub_count, word_count + sub_count);
	avl_out(tree_subj, odev, NULL);

	// close file
	fclose(odev);

	// return and exit
	return true;
}

// External funtion /////////////////////////////////////////
int parse()
{
	printf("\nParsing ... \n");

	// Nhap thu vien vao hash table
	char** hashTable = (char**)calloc(MODULE, sizeof(char*));
	if (hashTable == NULL)
	{
		perror("parse() : ");
		return false;
	}
	if (InHashTable(hashTable) == false)
		return false;

	// Xu ly loai bo ki tu ngan cach trong file PATH_IN,
		// luu noi dung vao file PATH_TEMPT
	if (delDelim(PATH_IN) == false)
		return false;

	// Mo file PATH_TEMPT
	FILE* idev = fopen(PATH_TEMPT, "rt");
	if (idev == NULL)
	{
		perror("parse() : ");
		return false;
	}

	char s[MAX_WORD_SIZE];
	// Khoi tao unique queue luu ket qua
	uqueue* uqRes = malloc(sizeof(uqueue));
	uqueue_init(uqRes, mkword, outword2, freeword, cmpword);
	// Lap trong khi nhap tu thanh cong
	while (feof(idev) == 0 && fscanf(idev, "%s", s) > 0)
	{
		// Tim tu trong hash table (hashTable[])
		if (findWord(hashTable, s) == false)
			// Neu khong co, them tu vao unique queue (uqRes)
			uqueue_push(uqRes, s);
	}
	// Mo file PATH_OUT va xuat tu trong unique queue
	FILE* odev = fopen(PATH_OUT, "wt");
	fprintf(odev, "%u\n", uqueue_size(uqRes));
	uqueue_out(uqRes, odev);
	// Dong file, giai phong bo nho
	fclose(idev);
	fclose(odev);
	freeHashT(hashTable);
	// Xoa file PATH_TEMPT
	remove(PATH_TEMPT);
	// end
	return true;
}

void test(void* val, void* item)
{

}

int refresh()
{
	printf("\nRefreshing .... \n");

	// Nhap du lieu vao cay AVL
	AVL* tree_subj = (AVL*)malloc(sizeof(AVL));
	if (tree_subj == NULL)
	{
		perror("refresh() : ");
		return false;
	}
	avl_init(tree_subj, mksubj, outsubj, freesubj, cmpsubj);
	if (InTreeSubj(tree_subj) == false)
		return false;
	// Xuat du lieu tu cay AVL
	if (OutTreeSubj(tree_subj) == false)
		return false;
	// Giai phong bo nho
	avl_free(&tree_subj);
	// Tra ve va ket thuc
	return true;
}
