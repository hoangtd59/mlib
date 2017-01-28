#include "stdafx.h"

#include "hash.h"
//#include <string.h>

// function : k mod N
hash_t hash_str(char* str, size_t module)
{
	hash_t res = 0;
	int n = strlen(str);
	while (--n >= 0)
		res = (res * 26 + str[n] - 'A') % module;
	return res;
}

// function : (R + (k mod R)) mod N
hash_t hash_str_sec(char* str, size_t module, size_t R)
{
	hash_t res = 0;
	int n = strlen(str);
	while (--n >= 0)
		res = (R + (res * 26 + str[n] - 'A') % R) % module;
	return res;
}