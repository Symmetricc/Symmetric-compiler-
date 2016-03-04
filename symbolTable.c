#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<stdbool.h>
#include"symbolTable.h"
#include"Header.h"


// Symbol's Functions 
symbolTable STInitialize(void)
{
	symbolTable ST;
	ST.scopesNumber = 0;
	ST.scope = (scopePTR)malloc(sizeof(Scope));
	ST.enter_scope = enter_scope;
	ST.find_symbol = find_symbol;
	ST.exit_scope = exit_scope;
	ST.print_ST = print_ST;
	ST.scope[ST.scopesNumber] = scopeInitialize(0);
	return ST;
}
Scope scopeInitialize(int address)
{
	Scope scope;
	scope.variableNumber = -1;
	scope.address = address;
	scope.var = (variablePTR)malloc(sizeof(Variable));
	scope.add_symbol = add_symbol;
	scope.check_scope = check_scope;
	return scope;
}
void enter_scope(symbolTablePTR self)
{
	int newAddress = self->scope[self->scopesNumber].address;
	self->scopesNumber++;
	self->scope = (scopePTR)realloc(self->scope, sizeof(Scope) * (self->scopesNumber + 1));
	self->scope[self->scopesNumber] = scopeInitialize(newAddress);
}
bool find_symbol(symbolTablePTR self,bool *constant)
{
	int scosNum = self->scopesNumber + 1;
	for (int i = 0; i < scosNum; i++)
	{
		int varsNum = self->scope[i].variableNumber + 1;
		for (int j = 0; j < varsNum; j++)
		{
			if (strcmp(self->findSymbolName, self->scope[i].var[j].varName) == 0)
			{
				*constant = self->scope[i].var[j].constant;
				return true;
			}
		}
	}
	return false;
}
void add_symbol(scopePTR self)
{
	// 0 character
	// 1 int
	// 2 bool
	self->variableNumber++;
	int currentVar = self->variableNumber;
	self->var = (variablePTR)realloc(self->var, sizeof(Variable) * (currentVar + 1));


	self->var[currentVar].varName = stringCopy(self->var[currentVar].varName, self->addSymbolName);

	self->var[currentVar].address = self->address;
	self->var[currentVar].value = self->addSymbolValue;
	self->var[currentVar].constant = self->addSymbolConstant;

	int size = self->addSymbolSize;
	if (self->addSymbolType == 0)
	{
		self->address = self->address + (1 * size);
		self->var[currentVar].varType = "character";
	}
	if (self->addSymbolType == 1)
	{
		self->address = self->address + (4 * size);
		self->var[currentVar].varType = "int";
	}
	if (self->addSymbolType == 2)
	{
		self->address = self->address + (1 * size);
		self->var[currentVar].varType = "bool";
	}
}
bool check_scope(scopePTR self, char *IDname)
{
	int varsNum = self->variableNumber + 1;
	for (int j = 0; j < varsNum; j++)
	{
		printf("%s %s\n", self->checkScopeName, self->var[j].varName);
		if (strcmp(self->checkScopeName, self->var[j].varName) == 0)
			return true;
	}
	return false;
}
void exit_scope(symbolTablePTR self)
{
	self->scopesNumber--;
	self->scope = (scopePTR)realloc(self->scope, sizeof(Scope) * (self->scopesNumber + 1));
}
void print_ST(symbolTablePTR self)
{
	int scosNum = self->scopesNumber + 1;
	for (int i = 0; i < scosNum; i++)
	{
		printf("SCOPE NUMBER:%d\n", i);
		int varsNum = self->scope[i].variableNumber + 1;
		for (int j = 0; j < varsNum; j++)
		{
			printf("%d", j);
			printf("- %s %s %d %d %d\n", self->scope[i].var[j].varName, self->scope[i].var[j].varType, self->scope[i].var[j].value, self->scope[i].var[j].constant, self->scope[i].var[j].address);
		}
	}
}
// add_symbol
// find_symbol
// check_scope
// enter_scope
// exit_scope

bool new_scope()
{
	globalST.enter_scope(&globalST);
	return true;
}
bool new_var(varPTR var)
{
	globalST.scope[globalST.scopesNumber].addSymbolName = stringCopy(globalST.scope[globalST.scopesNumber].addSymbolName, var->varName); // Placeholder
	globalST.scope[globalST.scopesNumber].addSymbolValue = var->value;// Placeholder  
	globalST.scope[globalST.scopesNumber].addSymbolConstant = var->constant;// Placeholder 
	globalST.scope[globalST.scopesNumber].addSymbolType = var->varType;// Placeholder 
	globalST.scope[globalST.scopesNumber].addSymbolSize = var->size;// Placeholder 

	globalST.scope[globalST.scopesNumber].add_symbol(&globalST.scope[globalST.scopesNumber]);

	var->varName = "";
	var->varType = 0;
	var->constant = false;
	var->value = 0;
	var->size = 1;
	return true;
}
bool exist_in_scope(varPTR var)
{
	globalST.scope[globalST.scopesNumber].checkScopeName = var->varName;
	if (globalST.scope[globalST.scopesNumber].check_scope(&globalST.scope[globalST.scopesNumber]))
	{
		printf("%s is already declared \n", var->varName);
		exit(1);
		return true;
	}
	return false;
}
bool exist_symbol_table(varPTR var)
{
	globalST.findSymbolName = stringCopy(globalST.findSymbolName, var->varName);
	printf("\n %s \n", var->varName);
	bool isConstant ;
	if (globalST.find_symbol(&globalST, &isConstant))
	{
		if (isConstant)
		{
			printf("Variable is Constant can't be changed \n");
			exit(1);
		}
		return true;
	}

	printf("%s was not declared\n", var->varName);
	exit(1);
}
bool delete_scope()
{
	globalST.exit_scope(&globalST);
	return true;
}
bool print_symbol_table()
{
	globalST.print_ST(&globalST);
	return true;
}
