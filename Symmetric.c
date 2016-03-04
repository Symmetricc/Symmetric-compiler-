#include"Header.h"
#include<stdio.h>
#include"symbolTable.h"

int main(int argc, char* argv[])
{
	globalST = STInitialize();

	if (argc != 3)
	{
		printf("Usage: Symmetric sourceFile \n");
		return 0;
	}
	fopen_s(&sourceFile, argv[1], "r");
	fopen_s(&TAC, argv[2], "w");
	printf("%d", program());
}