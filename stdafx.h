// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef _STDAFX_H_
#define _STDAFX_H_

#include "targetver.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "word.h"
#include "uqueue.h"
#include "AVL.h"
#include "hash.h"
#include "value.h"
#include "command.h"
#include "run.h"

#define PATH_IN "DATA.IN"
#define PATH_OUT "DATA.OUT"
#define PATH_LIB "LIB.INS"
#define PATH_TEMPT "TEMPT.OUT"

#define true 1
#define false 0

#define MAX_WORD 100000
#define MODULE 10000011
#define MAXR 97
#define MAX_WORD_SIZE 32

static int _argc = 1;
static char** _argv = NULL;

typedef struct {
	char str[8];
	void(*function)();
} command;

static char cmd[5] = { 0 };
static command arrcmd[] = {
	{ "-p", parse },
	{ "-r", refresh }
};

static void(*funtion)();

#endif;

// TODO: reference additional headers your program requires here
