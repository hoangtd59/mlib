#ifndef _WORD_H_
#define _WORD_H_

#include "stdafx.h"

int delDelim(const char* fileName);
void normalWord(char* word);
inline int isVowel(char ch);
inline int isConsonant(char ch);
int delSES(char* dest, const char* source);
int delING(char* dest, const char* source);
int delED(char* dest, const char* source);


#endif
