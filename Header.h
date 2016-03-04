#ifndef HEADER
#define HEADER
#include<stdio.h>
#include<stdbool.h>

#define MALLOC(type) ((type *)malloc(sizeof(type)))
#define MALLOC_ARRAY(number,type) ((type *)malloc(number * sizeof(type)))
#define RETURNPOINTER(position) fsetpos(sourceFile,&position)
#define SETPOINTER(position) fgetpos(sourceFile,&position)

#define inputsNumber 41 
#define BUFF_SIZE 16384

typedef enum { keyword, id, number, symbol, eof, error, character, str }TokenType;

typedef struct Token{
	char *lex;
	TokenType type;
	int TokenLine;
	int tokenValue;
}Token;


extern int table[61][27];
extern char input[inputsNumber];
FILE *sourceFile;
FILE *TAC;

// ------------------------------START SCANNER FUNCTIONS ------------------------------//
int GetIndex(char c);
Token GetNextToken();
Token lookAHead();
// ------------------------------END SCANNER FUNCTIONS ------------------------------//


// ------------------------------START PARSER FUNCTIONS ------------------------------//

// Helper's Functions
char *newTemp();
char *newLabel();
char *newSkip();
bool is_logic_op(void);
bool is_comparison_op(void);
bool is_char(bool);
bool is_ID(void);
bool is_number(bool);
void error_function(char, char);
bool is_type(int *);
bool terminated(void);
bool check_symbol(char *,bool);
bool check_keyword(char *, bool);
bool is_add_op(void);
bool is_mull_op(void);

// Grammar's Functions
bool program(void);
bool sequence_bar(void);
bool stmt_sequence(void);
bool simple_stmt(void);
bool term_statement(void);
bool statement(void);
bool if_stmt(void);
bool repeat_stmt(void);
bool write_stmt(void);
bool read_stmt(void);
bool declaration(void);
bool stmt_dcl(void);
bool arr_dcl(void);
bool definition(void);
bool stmt_def(void);
bool arr_def(void);
bool sub_char_asg(void);
bool sub_char_bar(void);
bool sub_asg(void);
bool sub_asg_bar(void);
bool assign_stmt(void);
bool char_asg(void);
bool char_arr_asg(void);
bool arr_asg(void);
bool stmt_asg(void);
bool expr(void);
bool logic_expr(void);
bool logic_expr_bar(void);
bool simple_expr(void);
bool simple_expr_bar(void);
bool term(void);
bool term_bar(void);
bool factor(void);
// ------------------------------END PARSER FUNCTIONS ------------------------------//


// ------------------------------START UTILITY FUNCTIONS ------------------------------//
char *stringCopy(char *, char *);
// ------------------------------END UTILITY FUNCTIONS ------------------------------//

#endif