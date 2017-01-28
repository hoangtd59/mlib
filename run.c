#include "stdafx.h"

#include "run.h"

void StartMessage(FILE * odev)
{
	fprintf(odev, "\n\n------------ Hello ! --------------\n\n");
}

void Input(FILE * idev, FILE * odev)
{
	fprintf(odev, "Mode : ");
	fscanf(idev, "%s", cmd);
}

int ValidData()
{
	int n = sizeof(arrcmd) / sizeof(arrcmd[0]);
	while (n >= 0)
	{
		if (strcmp(cmd, arrcmd[n].str) == 0)
		{
			funtion = arrcmd[n].function;
			return true;
		}
		n--;
	}
	return false;
}

void ErrorMessage(FILE * odev)
{
	fprintf(odev, "\nInvalid command !\n");
}

void Processing()
{
	funtion();
}

void Output(FILE * odev)
{
	fprintf(odev, "\nDone !\n");
}

int AskToContinue(FILE * idev, FILE * odev)
{
	char chose;
	fprintf(odev, "\nPress 'y' or 'Y' to continue, others to exit !\n");
	fscanf(idev, "\n%c", &chose);
	return (chose == 'y' || chose == 'Y');
}

void Run(FILE * idev, FILE * odev)
{
	int chose = false;
	do {
		StartMessage(odev);
		Input(idev, odev);
		if (ValidData() == false)
		{
			ErrorMessage(odev);
			continue;
		}
		Processing();
		Output(odev);
		chose = AskToContinue(idev, odev);
	} while (chose == true);
}
