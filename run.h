#ifndef _RUN_H_
#define _RUN_H_

#include "stdafx.h"

void StartMessage(FILE* odev);
void Input(FILE* idev, FILE* odev);
int ValidData();
void ErrorMessage(FILE* odev);
void Processing();
void Output(FILE* odev);
int AskToContinue(FILE* idev, FILE* odev);
void Run(FILE* idev, FILE* odev);

#endif