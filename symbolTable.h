#ifndef SYMBOLTABLE
#define SYMBOLTABLE

#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>

typedef struct _Var Var, *varPTR;
typedef struct _Variable Variable, *variablePTR;
typedef struct _Scope Scope, *scopePTR;
typedef struct _symbolTable symbolTable, *symbolTablePTR;
struct _Var{
	char *varName;
	int varType;
	int value;
	bool constant;
	int size;
};
struct _Variable{
	char *varName;
	char *varType;
	int address;
	int value;
	bool constant;
};
// Structure that represent scope,
// and has every function that modfiy a scope,
// and has every attribute related to a scope,
// and it has variable array.
struct _Scope{
	int address;
	int variableNumber;
	char *addSymbolName;
	int addSymbolValue;
	bool addSymbolConstant;
	int addSymbolType;
	int addSymbolSize;
	int checkScopeName;
	variablePTR var;
	void(*add_symbol)(scopePTR);
	bool(*check_scope)(scopePTR);

};
// Structure that represent symbol table,
// and has every function modify the ST,
// every attribute related to ST,
// and has the scope array.
struct _symbolTable{
	int scopesNumber;
	char *findSymbolName;
	Scope *scope;
	void(*enter_scope)(symbolTablePTR);
	bool(*find_symbol)(symbolTablePTR,bool *);
	void(*exit_scope)(symbolTablePTR);
	void(*print_ST)(symbolTablePTR);

};


// Symbole Table Functinos
symbolTable STInitialize(void);
Scope scopeInitialize(int);
void enter_scope(symbolTable);
bool check_scope(scopePTR);
void add_symbol(scopePTR);
bool find_symbol(symbolTablePTR);
void enter_scope(symbolTablePTR);
void exit_scope(symbolTablePTR);
void print_ST(symbolTablePTR);


// Symbol Table API

symbolTable globalST;

bool new_scope();
bool new_var(varPTR);
bool exist_in_scope(varPTR);
bool exist_symbol_table(varPTR);
bool delete_scope();
bool print_symbol_table();
#endif

