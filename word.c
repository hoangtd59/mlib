#include "stdafx.h"
#include "word.h"
#include <ctype.h>

int delDelim(const char* fileName)
{
	FILE* idev = fopen(fileName, "rt");
	FILE* odev = fopen(PATH_TEMPT, "wt");
	if (idev == NULL)
		return false;
	char ch;
	size_t pos;
	while (feof(idev) == 0)
	{
		ch = fgetc(idev);
		if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_')
		{
			fputc(ch, odev);
		}
		else
		{
			ch = ' ';
			fputc(ch, odev);
		}
	}
	fclose(idev);
	fclose(odev);
	return true;
}

void normalWord(char* word)
{
	size_t n = strlen(word);
	size_t i;
	for (i = 0; i < n; i++)
	{
		if (islower(word[i]))
		{
			for (i = 0; i < n; i++)
				word[i] = tolower(word[i]);
			return;
		}
	}
}

inline int isVowel(char ch)
{
	return (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u' || ch == 'y');
}

inline int isConsonant(char ch)
{
	return !isVowel(ch);
}

int delSES(char* dest, const char* source)
{
	size_t n = strlen(source);
	if (n >= 3 && (source[n - 1] == 's'))
	{
		strcpy(dest, source);
		if (n >= 4 && dest[n - 2] == 'e')
		{
			switch (dest[n - 3])
			{
			case 'i': // _ies -> _y
				dest[n - 2] = '\0';
				dest[n - 3] = 'y';
				return 1;
			case 'h':
				if (n >= 6 && (dest[n - 4] == 's' || dest[n - 4] == 'c')); // _shes, _ches
				break;
			case 'o':  // _oes
				break;
			case 's': case 'x': case 'z': //  _ses, _xes, _zes
				if (n < 5)
					return 0;
				break;
			default:
				return 0;
			}
			dest[n - 2] = '\0'; // _es -> _
		}
		else
		{
			if (dest[n - 2] == 's') // _ss
				return 0;
			else if (isConsonant(dest[n - 2]))// _cs -> _c
				dest[n - 1] = '\0';
		}
		return 1;
	}
	return 0;
}

int delING(char* dest, const char* source)
{
	size_t n = strlen(source);
	if (n >= 5 && source[n - 3] == 'i' && source[n - 2] == 'n' && source[n - 1] == 'g')
	{
		strcpy(dest, source);
		if (dest[n - 4] == 'y' && isConsonant(dest[n - 5])) // _cying -> _cie
		{
			dest[n - 4] = 'i';
			dest[n - 3] = 'e';
			dest[n - 2] = '\0';
			return 1;
		}
		if (n >= 7 && dest[n - 4] == dest[n - 5] && isConsonant(dest[n - 4] && isConsonant(dest[n - 5]))) // _ccing -> _c
		{
			dest[n - 4] = '\0';
			return 1;
		}
		dest[n - 3] = '\0'; // _ing -> _
		return 1;
	}
	return 0;
}

int delED(char* dest, const char* source)
{
	size_t n = strlen(source);
	if (n >= 4 && source[n - 2] == 'e' && source[n - 1] == 'd')
	{
		strcpy(dest, source);
		if (dest[n - 3] == 'e') // _eed -> _ee
		{
			dest[n - 1] = '\0';
			return 1;
		}
		if (dest[n - 3] == 'i' && isConsonant(dest[n - 4])) // _ied -> _y
		{
			dest[n - 3] = 'y';
			dest[n - 2] = '\0';
			return 1;
		}
		if (n >= 6 && (dest[n - 3] == dest[n - 4]) && isConsonant(dest[n - 3] && isConsonant(dest[n - 4]))) // _cced -> _c
		{
			dest[n - 3] = '\0';
			return 1;
		}
		if (n >= 6 && isConsonant(dest[n - 3] && isVowel(dest[n - 4]))) // _vced -> _vce
		{
			dest[n - 1] = '\0';
			return 1;
		}
		dest[n - 2] = '\0'; // _ed -> _
		return 1;
	}
	return 0;
}
