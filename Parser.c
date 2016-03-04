#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include"Header.h"
#include"symbolTable.h"

Var parserVariables;
char *numberOrID;
char *oper;
int variableNumber;
int labelNumber;
char *val;
char *charHolder;
char *newTemp()
{
	char varNum[10];
	_itoa_s(variableNumber++, varNum, 10, 10);
	char *temp;
	temp = (char *)malloc(sizeof(char) * 10);
	_snprintf_s(temp, 10, 12, "T%s", varNum);
	return temp;
}
char *newLabel()
{
	char varNum[10];
	_itoa_s(labelNumber, varNum, 10, 10);
	char *temp;
	temp = (char *)malloc(sizeof(char) * 30);
	_snprintf_s(temp, 10, 12, "Label%s", varNum);
	return temp;
}
char *newSkip()
{
	char varNum[10];
	_itoa_s(labelNumber++, varNum, 10, 10);
	char *temp;
	temp = (char *)malloc(sizeof(char) * 30);
	_snprintf_s(temp, 10, 12, "Skip%s", varNum);
	return temp;
}
bool is_logic_op(void)
{
	printf("       is_logic_op\n");
	Token tok = lookAHead();
	if (tok.type != symbol)
		return false;
	if (strcmp(tok.lex, "|") == 0)
	{
		GetNextToken();
		tok = lookAHead();
		if (strcmp(tok.lex, "|") == 0)
		{
			oper = stringCopy(oper, "||");
			GetNextToken();
			return true;
		}
		error_function("A symbol is expected", "");
	}
	else if (strcmp(tok.lex, "") == 0)
	{
		GetNextToken();
		tok = lookAHead();
		if (strcmp(tok.lex, "&") == 0)
		{
			oper = stringCopy(oper, "&&");
			GetNextToken();
			return true;
		}
		error_function("A symbol is expected", "");
	}
	return false;
}
bool is_EOF(void)
{
	Token tok = GetNextToken();
	if (tok.type != eof)
		error_function("UNKNOWN ERROR", "");
	return true;
}
bool is_add_op(void)
{
	printf("        is_add_op \n");
	Token tok = lookAHead();
	if (tok.type != symbol)
		return false;
	if (strcmp(tok.lex, "+") == 0 || strcmp(tok.lex, "-") == 0)
	{
		oper = stringCopy(oper, tok.lex);
		GetNextToken();
		return true;
	}
	return false;
}
bool is_mull_op(void)
{
	printf("         is_mull_op \n");
	Token tok = lookAHead();
	if (tok.type != symbol)
		return false;
	if (strcmp(tok.lex, "*") == 0 || strcmp(tok.lex, "/") == 0)
	{
		oper = stringCopy(oper, tok.lex);
		GetNextToken();
		return true;
	}

	return false;
}
bool is_comparison_op(void)
{
	printf("       is_comparison_op \n");
	Token tok = lookAHead();
	if (tok.type != symbol)
		return false;
	if (strcmp(tok.lex, "=") == 0)
	{
		GetNextToken();
		return true;
	}
	if (strcmp(tok.lex, "<") == 0)
	{
		GetNextToken();
		tok = lookAHead();
		if (strcmp(tok.lex, "=") == 0)
		{
			oper = stringCopy(oper, "<=");
			GetNextToken();
			return true;
		}
		oper = stringCopy(oper, "<");
		return true;
	}
	else if (strcmp(tok.lex, ">") == 0)
	{
		GetNextToken();
		tok = lookAHead();
		if (strcmp(tok.lex, "=") == 0)
		{
			oper = stringCopy(oper, ">=");
			GetNextToken();
			return true;
		}
		oper = stringCopy(oper, ">");
		return true;
	}
	return false;
}
bool is_char(bool errorFlag)
{
	Token tok = GetNextToken();
	if (tok.type != character && errorFlag == true)
		error_function("A charcter is expected", "");
	if (tok.type != character && errorFlag == false)
		return false;
	charHolder = stringCopy(charHolder, tok.lex);
	return true;
}
bool is_number(bool errorFlag)
{
	printf("         is_number \n");
	Token tok = GetNextToken();
	if (tok.type != number && errorFlag == true)
		error_function("A number is expected", "");
	if (tok.type != number && errorFlag == false)
		return false;
	parserVariables.value = tok.tokenValue;
	numberOrID = stringCopy(numberOrID, tok.lex);
	return true;
}
bool is_ID(bool errorFlag)
{
	printf("is_ID()\n");
	Token tok = GetNextToken();
	if (tok.type != id && errorFlag == true)
		error_function("An identifier is expected", "");
	if (tok.type != id && errorFlag == false)
		return false;
	parserVariables.varName = stringCopy(parserVariables.varName, tok.lex);
	numberOrID = stringCopy(numberOrID, tok.lex);
	return true;
}
bool is_type(int *tokType)
{
	// 0 character
	// 1 int
	// 2 bool
	Token tok = GetNextToken();
	char *lxl = "";
	lxl = stringCopy(lxl, tok.lex);
	if (strcmp(tok.lex, "inttni") == 0 || strcmp(tok.lex, "bood") == 0 || strcmp(tok.lex, "chhc") == 0)
	{
		if (strcmp(lxl, "chhc") == 0)
			*tokType = 0;
		else if (strcmp(lxl, "inttni") == 0)
			*tokType = 1;
		else if (strcmp(lxl, "bood") == 0)
			*tokType = 2;
		parserVariables.varType = *tokType;
		return true;

	}
	return false;
}
bool terminated(void)
{
	printf("terminated \n");
	Token tok = lookAHead();
	if (tok.type == keyword && (strcmp(tok.lex, "lool") == 0 || strcmp(tok.lex, "if") == 0))
		return false;
	return true;
}
bool check_symbol(char *smbl, bool errorFlag)
{
	char errorMSG[30] = "";
	_snprintf_s(errorMSG, 30, 30, "%s is Expected", smbl);
	Token tok = GetNextToken();
	if ((strcmp(smbl, tok.lex) != 0 || tok.type != symbol) && errorFlag == true)
		error_function(errorMSG, "");
	if ((tok.type != symbol || strcmp(tok.lex, smbl) != 0) && errorFlag == false)
		return false;
	return true;
}
bool check_keyword(char *KW, bool errorFlag)
{
	Token tok = GetNextToken();

	char errorMSG[30] = "";
	_snprintf_s(errorMSG, 30, 30, "%s is Expected", KW);
	if ((strcmp(KW, tok.lex) != 0 || tok.type != keyword) && errorFlag == true)
		error_function(errorMSG, "");
	if ((strcmp(KW, tok.lex) != 0 || tok.type != keyword) && errorFlag == false)
		return false;
	if (strcmp(KW, "connoc") == 0)
		parserVariables.constant = true;
	return true;
}
bool is_string(void)
{
	printf("is_string \n");
	Token tok = GetNextToken();
	if (tok.type == str)
	{
		fprintf(TAC, "%s \n", tok.lex);
		return true;
	}

	return false;
}
void error_function(char *errorMSG, char *extra)
{
	printf("%s", errorMSG);
	printf(" ");
	printf("%s", extra);

	printf("!\n");
	exit(1);
}

bool program(void)
{
	variableNumber = 0;
	labelNumber = 0;
	printf("program \n");
	return stmt_sequence() && is_EOF();
}
bool stmt_sequence(void)
{
	return simple_stmt() && sequence_bar();
}
bool sequence_bar(void)
{
	printf("  sequence_bar \n");
	fpos_t seqPosition;
	SETPOINTER(seqPosition);
	return simple_stmt() && sequence_bar()
		|| (RETURNPOINTER(seqPosition), true);

}
bool simple_stmt(void)
{
	parserVariables.varName = "";
	parserVariables.varType = 0;
	parserVariables.constant = false;
	parserVariables.value = 0;
	parserVariables.size = 1;

	printf("  simple_stmt \n");
	fpos_t simplePosition;
	SETPOINTER(simplePosition);
	return (terminated() && term_statement() && check_symbol(";", true))
		|| (RETURNPOINTER(simplePosition), statement());
}
bool term_statement(void)
{
	printf("   term_statement \n");
	fpos_t termPosition;
	SETPOINTER(termPosition);
	;
	return (assign_stmt())
		|| (RETURNPOINTER(termPosition), read_stmt())
		|| (RETURNPOINTER(termPosition), write_stmt())
		|| (RETURNPOINTER(termPosition), definition())
		|| (RETURNPOINTER(termPosition), declaration());
}
bool statement(void)
{
	printf("   statement \n");
	fpos_t stmtPosition;
	SETPOINTER(stmtPosition);
	return (if_stmt())
		|| (RETURNPOINTER(stmtPosition), repeat_stmt());
}
bool if_stmt(void)
{
	printf("    if_stmt \n");
	fpos_t ifPosition;
	char *skip = "";
	char *label= "";
	return (check_keyword("if", false) && new_scope() && (label = stringCopy(label,newLabel())) && (skip = stringCopy(skip,newSkip())) && check_symbol("(", true) && fprintf(TAC, "if_false ") && expr() && fprintf(TAC,"GOTO %s\n",skip) && check_symbol(")", true) && check_keyword("then", true) && stmt_sequence() && fprintf(TAC,"GOTO %s \n",label) && check_keyword("neht", true) && delete_scope() && ((SETPOINTER(ifPosition), check_keyword("else", false) && fprintf(TAC,"%s\n",skip) && new_scope() && check_keyword("then", true) && stmt_sequence() && check_keyword("neht", true) && delete_scope() && fprintf(TAC,"%s",label))
																																																																														 || (RETURNPOINTER(ifPosition), true)));
}
bool repeat_stmt(void)
{
	printf("    repeat_stmt \n");
	char *label = "";
	char *skip = "";
	return check_keyword("lool", false) && new_scope() && (label = stringCopy(label, newLabel())) && (skip = stringCopy(skip, newSkip())) && fprintf(TAC, "%s:\n", label) && check_symbol("(", true) && fprintf(TAC, "if_false ") && expr() && fprintf(TAC, "GOTO %s\n", skip) && check_symbol(")", true) && check_keyword("do", true) && stmt_sequence() && fprintf(TAC,"GOTO %s\n",label)&&check_keyword("od", true) && delete_scope()  && fprintf(TAC,"%s\n",skip) ;
}
bool write_stmt(void)
{
	printf("    write_stmt \n");
	fpos_t writePosition;
	SETPOINTER(writePosition);
	return (check_keyword("right", false) && fprintf(TAC, "Write ") && check_symbol("(", true) && is_string() && check_symbol(")", true))
		|| (RETURNPOINTER(writePosition), check_keyword("right ", false) && check_symbol("(", true) && expr() && check_symbol(")", true));

}
bool read_stmt(void)
{
	printf("    read_stmt \n");
	return check_keyword("left", false) && fprintf(TAC,"Read ") && check_symbol("(", true) && is_ID(true) &&fprintf(TAC,"%s \n",parserVariables.varName) && check_symbol(")", true);
}
bool declaration(void)
{
	printf("    declaration \n");
	fpos_t dclPosition;
	SETPOINTER(dclPosition);
	return (arr_dcl())
		|| (RETURNPOINTER(dclPosition), stmt_dcl());
}
bool stmt_dcl(void)
{
	printf("     stmt_dcl\n");
	int tokType;
	return is_type(&tokType) && is_ID(true) && !exist_in_scope(&parserVariables) && new_var(&parserVariables);
}
bool arr_dcl(void)
{
	printf("     arr_dcl\n");
	int tokType;
	return is_type(&tokType) && is_ID(true) && check_symbol("[", false) && is_number(true) && check_symbol("]", true) && !exist_in_scope(&parserVariables) && (parserVariables.size = parserVariables.value, new_var(&parserVariables));
}
bool definition(void)
{
	printf("    definition\n");
	fpos_t defPosition;
	SETPOINTER(defPosition);
	return (arr_def())
		|| (RETURNPOINTER(defPosition), stmt_def());
}
bool stmt_def(void)
{
	printf("     stmt_def\n");
	fpos_t stmtDefPosition;
	SETPOINTER(stmtDefPosition);
	int tokType;
	return (is_type(&tokType) && tokType == 0 && check_keyword("connoc", false) && is_ID(true)  && !exist_in_scope(&parserVariables) && check_symbol(":", false) && fprintf(TAC,"%s = ",parserVariables.varName)&& is_char(true) && fprintf(TAC,"%s",charHolder) && new_var(&parserVariables))
		|| (RETURNPOINTER(stmtDefPosition), is_type(&tokType) && tokType == 0 && is_ID(true) && !exist_in_scope(&parserVariables) && check_symbol(":", false) && fprintf(TAC,"%s = ",parserVariables.varName)&& is_char(true) && fprintf(TAC,"%s",charHolder) && new_var(&parserVariables))
		|| (RETURNPOINTER(stmtDefPosition), is_type(&tokType) && check_keyword("connoc", false) && is_ID(true) && !exist_in_scope(&parserVariables) && check_symbol(":", true) && fprintf(TAC,"%s = ",parserVariables.varName)&& expr() && new_var(&parserVariables))
		|| (RETURNPOINTER(stmtDefPosition), is_type(&tokType) && is_ID(true) && !exist_in_scope(&parserVariables) && check_symbol(":", false) && fprintf(TAC,"%s = ",parserVariables.varName)&& expr() && new_var(&parserVariables));
}
bool arr_def(void)
{
	printf("     arr_def\n");
	fpos_t arrDefPosition;
	SETPOINTER(arrDefPosition);
	int tokType;
	return (is_type(&tokType) && tokType == 0 && is_ID(false) && !exist_in_scope(&parserVariables) && check_symbol("[", false) && is_number(true) && check_symbol("]", true) && (parserVariables.size = parserVariables.value, new_var(&parserVariables)) && check_symbol(":", true) && check_symbol("{", true) && sub_char_asg() && check_symbol("}", true))
		|| (RETURNPOINTER(arrDefPosition), is_type(&tokType) && is_ID(false) && check_symbol("[", false) && is_number(true) && check_symbol("]", true) && check_symbol(":", false) && check_symbol("{", true) && sub_asg() && check_symbol("}", true));

}
bool sub_char_asg(void)
{
	printf("      sub_char_asg \n");
	return is_char(false) && sub_char_bar();
}
bool sub_char_bar(void)
{
	printf("       sub_char_bar \n");
	fpos_t subCharPosition;
	SETPOINTER(subCharPosition);
	return check_symbol(",", false) && is_char(true) && sub_char_bar()
		|| (RETURNPOINTER(subCharPosition), true);
}
bool sub_asg(void)
{
	printf("      sub_asg \n");
	return  expr() && sub_asg_bar();
}
bool sub_asg_bar(void)
{
	printf("       sub_asg_bar \n");
	fpos_t subAsgPosition;
	SETPOINTER(subAsgPosition);
	return check_symbol(",", false) && expr() && sub_asg_bar()
		|| (RETURNPOINTER(subAsgPosition), true);
}
bool assign_stmt(void)
{
	printf("    assign_stmt \n");
	fpos_t asgStmtPosition;
	SETPOINTER(asgStmtPosition);
	return (char_asg())
		|| (RETURNPOINTER(asgStmtPosition), char_arr_asg())
		|| (RETURNPOINTER(asgStmtPosition), arr_asg())
		|| (RETURNPOINTER(asgStmtPosition), stmt_asg());
}
bool char_asg(void)
{
	printf("      char_asg \n");
	return is_ID(false) && check_symbol(":", false) && fprintf(TAC,"%s = ",parserVariables.varName) && exist_symbol_table(&parserVariables) && is_char(false) && fprintf(TAC,"%s \n",charHolder);
}
bool char_arr_asg(void)
{
	printf("      arr_asg \n");
	return is_ID(false) && check_symbol("[", false) && is_number(true) && check_symbol("]", true) && fprintf(TAC,"%s[%d] = ",parserVariables.varName,parserVariables.value) && check_symbol(":", false) && exist_symbol_table(&parserVariables) && is_char(false) && fprintf(TAC,"%s \n",charHolder);
}
bool arr_asg(void)
{
	printf("      arr_asg \n");
	return is_ID(false) && check_symbol("[", false) && is_number(true) && check_symbol("]", true) && check_symbol(":", false) && exist_symbol_table(&parserVariables) && expr();
}
bool stmt_asg(void)
{
	printf("      stmt_asg \n");
	return is_ID(false) && check_symbol(":", true) && exist_symbol_table(&parserVariables) && expr();
}
bool expr(void)
{
	printf("       expr \n");
	fpos_t exprPosition;
	fpos_t exprPosition2;
	char *leftVal = "";
	char *rightVal = "";
	char *currOper = "";
	SETPOINTER(exprPosition);
	return (check_symbol("#", false) && expr() && fprintf(TAC, "!%s\n", val))
		|| (RETURNPOINTER(exprPosition), logic_expr() && (leftVal = stringCopy(leftVal, val)) && (SETPOINTER(exprPosition2),(is_comparison_op() || is_logic_op() || is_mull_op() || is_add_op()) && (currOper = stringCopy(currOper, oper)) && logic_expr() && (rightVal = stringCopy(rightVal, val)) && fprintf(TAC, "%s %s %s\n", leftVal, currOper, rightVal))
		|| (RETURNPOINTER(exprPosition2), true && fprintf(TAC, "%s\n",val)));
}

bool logic_expr(void)
{
	printf("      logic_expr()\n");
	char *val = "";
	return simple_expr() && logic_expr_bar();
}
bool logic_expr_bar(void)
{
	printf("		logic_expr_bar()\n");
	fpos_t logicExprPosition;
	char *newValue = "";
	char *currOper = "";
	char *currVal = "";
	char *logicVal = "";
	SETPOINTER(logicExprPosition);
	return (is_logic_op() && (currOper = stringCopy(currOper, oper)) && (newValue = newTemp()) && (logicVal = stringCopy(logicVal, val)) && simple_expr() && fprintf(TAC, "%s = %s %s %s \n", newValue, currVal, oper, logicVal) && (val = stringCopy(val, newValue)) && logic_expr_bar())
		|| (RETURNPOINTER(logicExprPosition), true);
}
bool simple_expr(void)
{
	printf("       simple_expr \n");
	return term()  && simple_expr_bar();
}
bool simple_expr_bar(void)
{
	printf("        simple_expr_bar \n");
	fpos_t exprBarPosition;
	char *newValue = "";
	char *currOper = "";
	char *currVal = "";
	char *exprVal = "";
	currVal = stringCopy(currVal, val);
	SETPOINTER(exprBarPosition);
	return (is_add_op() && (currOper = stringCopy(currOper, oper)) && (newValue = newTemp()) && term() && (exprVal = stringCopy(exprVal, val)) && fprintf(TAC, "%s = %s %s %s\n", newValue, currVal, currOper, exprVal) && (val = stringCopy(val, newValue)) && simple_expr_bar())
		|| (RETURNPOINTER(exprBarPosition), true);
}
bool term()
{
	printf("        term %d \n", val);
	return factor() && term_bar();
}
bool term_bar(void)
{
	printf("         term_bar \n");
	fpos_t termBarPosition;
	char *newValue = "";
	char *termVal = "";
	char *currVal = "";
	char *currOper = "";
	currVal = stringCopy(currVal, val);
	SETPOINTER(termBarPosition);
	return (is_mull_op() && (currOper = stringCopy(currOper, oper)) && (newValue = newTemp()) && factor() && (termVal = stringCopy(termVal, val)) && fprintf(TAC, "%s = %s %s %s\n", newValue, currVal, currOper, termVal) && (val = stringCopy(val, newValue)) && term_bar())
		|| (RETURNPOINTER(termBarPosition), true);
}
bool factor(void)
{
	printf("         factor \n");
	fpos_t factorPosition;
	SETPOINTER(factorPosition);
	return ((is_ID(false) && exist_symbol_table(&parserVariables) && check_symbol("[", false) && is_number(true) && check_symbol("]", true))
		|| (RETURNPOINTER(factorPosition), is_ID(false) &&exist_symbol_table(&parserVariables))
		|| (RETURNPOINTER(factorPosition), check_symbol("(", false) && expr() && check_symbol(")", false))
		|| (RETURNPOINTER(factorPosition), is_number(true))) && (val = stringCopy(val, numberOrID));
}