%{
#include <sys/types.h>
#include "lex.yy.c"
#include "GrammarTree.h"
%}

/* declared types */
%union {
	int type_int;
	float type_float;
	double type_double;
}

%token <type_int> INT
%token <type_float> FLOAT
%token ID
%token SEMI COMMA DOT
%token ASSIGNOP RELOP
%token PLUS MINUS DIV STAR
%token AND OR NOT
%token TYPE
%token LP RP LB RB LC RC
%token STRUCT RETURN IF ELSE WHILE

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%left ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right MINUS NOT
%left LP RP LB RB DOT
%%
Program		: ExtDefList		/*High-level Definitions*/
		;
ExtDefList	: /* empty */
		| ExtDef ExtDefList	
		;
ExtDef		: Specifier ExtDecList SEMI
		| Specifier SEMI
		| Specifier FunDec CompSt
		;
ExtDecList	: VarDec
		| VarDec COMMA ExtDecList
		;

Specifier	: TYPE			/*Specifiers*/
		| StructSpecifier
StructSpecifier	: STRUCT OptTag LC DefList RC
		| STRUCT Tag
		;
OptTag		: /* empty */
		| ID
		;
Tag		: ID
		;

VarDec		: ID			/*Declarators*/
		| VarDec LB INT RB
		;
FunDec		: ID LP VarList RP
		| ID LP RP
		;
VarList		: ParamDec COMMA VarList
		| ParamDec
		;

ComptSt		: LC DefList StmtList RC/*Statements*/
		;
StmtList	: /* empty */
		| Stmt StmtList
Stmt		: Exp SEMI
		| CompSt
		| RETURN Exp SEMI
		| IF LP Exp RP Stmt	%prec LOWER_THAN_ELSE
		| IF LP Exp RP Stmt ELSE Stmt
		| WHILE LP Exp RP Stmt
		;

DefList		: /* empty */		/*Local Definitions*/
		| Def DefList
		;
Def		: Specifier DecList SEMI
		;
DecList		: Dec
		| Dec COMMA DecList
		;
Dec		: VarDec
		| VarDec ASSIGNOP Exp
		;

Exp		: Exp ASSIGNOP Exp	/*Expressions*/
		| Exp AND Exp
		| Exp OR Exp
		| Exp RELOP Exp
		| Exp PLUS Exp
		| Exp MINUS Exp
		| Exp STAR Exp
		| Exp DIV Exp
		| LP Exp RP
		| MINUS Exp
		| NOT Exp
		| ID LP Args RP
		| Exp LB Exp RB
		| Exp DOT ID
		| ID
		| INT
		| FLOAT
		;
Args		: Exp COMMA Args
		| Exp
		;
%%
yyerror(char *msg){
	fprintf(stderr, "error: %s\n", msg);
}
