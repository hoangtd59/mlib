#ifndef _HASH_H_
#define _HASH_H_

#include "stdafx.h"
//#include <stdio.h>


typedef size_t hash_t;

hash_t hash_str(char* str, size_t module);
hash_t hash_str_sec(char* str, size_t module, size_t R);

#endif