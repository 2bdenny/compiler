%{
#define YYSTYPE char*
#define YYDEBUG 1
#include <stdarg.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include "lex.yy.c"
#include "GrammarTree.h"
Leaf *reduce(int len, ...);
Leaf *reduce0(char *name, int line);
%}

/* declared types */
%token INT
%token FLOAT
%token ID
%token SEMI COMMA DOT
%token ASSIGNOP RELOP
%token PLUS MINUS DIV STAR HIGH_MINUS
%token AND OR NOT
%token TYPE
%token LP RP LB RB LC RC
%token STRUCT RETURN IF ELSE WHILE

%nonassoc EDF_ERR
%nonassoc VDC_ERR COM_ERR STR_ERR
%nonassoc STM_ERR FUN_ERR
%nonassoc RB_ERR RP_ERR
%nonassoc DEF_ERR

%nonassoc LL_THAN_ELSE
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT HIGH_MINUS
%left LP RP LB RB DOT
%%
Program		: ExtDefList			{$$ = (char *)reduce(3, "Program", @$.first_line, $1); /*display((Leaf *)($$), 0); destroyForest();*/}
		;
ExtDefList	: ExtDef ExtDefList		{$$ = (char *)reduce(4, "ExtDefList", @$.first_line, $1, $2);}
		| /* empty */			{$$ = (char *)reduce0("ExtDefList", @$.first_line);}
		;
ExtDef		: Specifier ExtDecList SEMI	{$$ = (char *)reduce(5, "ExtDef", @$.first_line, $1, $2, $3);}
		| Specifier SEMI		{$$ = (char *)reduce(4, "ExtDef", @$.first_line, $1, $2);}
		| Specifier FunDec CompSt	{$$ = (char *)reduce(5, "ExtDef", @$.first_line, $1, $2, $3);}
		| error SEMI %prec EDF_ERR	{$$ = (char *)reduce0("ExtDef", @$.first_line); meetError(); printf("def error\n");}

		;
ExtDecList	: VarDec			{$$ = (char *)reduce(3, "ExtDecList", @$.first_line, $1);}
		| VarDec COMMA ExtDecList	{$$ = (char *)reduce(5, "ExtDecList", @$.first_line, $1, $2, $3);}
		;

Specifier	: TYPE				{$$ = (char *)reduce(3, "Specifier", @$.first_line, $1);}
		| StructSpecifier		{$$ = (char *)reduce(3, "Specifier", @$.first_line, $1);}
		;
StructSpecifier	: STRUCT OptTag LC DefList RC	{$$ = (char *)reduce(7, "StructSpecifier", @$.first_line, $1, $2, $3, $4, $5);}
		| STRUCT Tag			{$$ = (char *)reduce(4, "StructSpecifier", @$.first_line, $1, $2);}
		| error RC %prec STR_ERR	{$$ = (char *)reduce0("StructSpecifier", @$.first_line); meetError(); printf("struct error\n");}

		;
OptTag		: ID				{$$ = (char *)reduce(3, "OptTag", @$.first_line, $1);}
		| /* empty */			{$$ = (char *)reduce0("OptTag", @$.first_line);}
		;
Tag		: ID				{$$ = (char *)reduce(3, "Tag", @$.first_line, $1);}
		;

VarDec		: ID				{$$ = (char *)reduce(3, "VarDec", @$.first_line, $1);}
		| VarDec LB INT RB		{$$ = (char *)reduce(6, "VarDec", @$.first_line, $1, $2, $3, $4);}
		| error RB %prec VDC_ERR	{$$ = (char *)reduce0("VarDec", @$.first_line); meetError(); printf("want int\n");}

		;
FunDec		: ID LP VarList RP		{$$ = (char *)reduce(6, "FunDec", @$.first_line, $1, $2, $3, $4);}
		| ID LP RP			{$$ = (char *)reduce(5, "FunDec", @$.first_line, $1, $2, $3);}
		| error RP %prec FUN_ERR	{$$ = (char *)reduce0("FunDec", @$.first_line); meetError(); printf("Expect \( or VarList\n");}
		;
VarList		: ParamDec COMMA VarList	{$$ = (char *)reduce(5, "VarList", @$.first_line, $1, $2, $3);}
		| ParamDec			{$$ = (char *)reduce(3, "ParamDec", @$.first_line, $1);}
		;
ParamDec	: Specifier VarDec		{$$ = (char *)reduce(4, "ParamDec", @$.first_line, $1, $2);}
		;

CompSt		: LC DefList StmtList RC	{$$ = (char *)reduce(6, "CompSt", @$.first_line, $1, $2, $3, $4);}
		| error RC %prec COM_ERR	{$$ = (char *)reduce0("CompSt", @$.first_line); meetError(); printf("Expect ; or StmtList before '}'\n");}
		;
StmtList	: Stmt StmtList			{$$ = (char *)reduce(4, "StmtList", @$.first_line, $1, $2);}
		| /* empty */			{$$ = (char *)reduce0("StmtList", @$.first_line);}
		;
Stmt		: Exp SEMI			{$$ = (char *)reduce(4, "Stmt", @$.first_line, $1, $2);}
		| CompSt			{$$ = (char *)reduce(3, "Stmt", @$.first_line, $1);}
		| RETURN Exp SEMI		{$$ = (char *)reduce(5, "Stmt", @$.first_line, $1, $2, $3);}
		| IF LP Exp RP Stmt	%prec LOWER_THAN_ELSE	{$$ = (char *)reduce(5, "Stmt", @$.first_line, $1, $2, $3);}
		| IF LP Exp RP Stmt ELSE Stmt	{$$ = (char *)reduce(6, "Stmt", @$.first_line, $1, $2, $3, $4);}
		| WHILE LP Exp RP Stmt		{$$ = (char *)reduce(7, "Stmt", @$.first_line, $1, $2, $3, $4, $5);}
		| error SEMI %prec STM_ERR	{$$ = (char *)reduce0("Stmt", @$.first_line); meetError(); printf("Expect statement\n");}
		;

DefList		: Def DefList			{$$ = (char *)reduce(4, "DefList", @$.first_line, $1, $2);}
		| /* empty */ 			{$$ = (char *)reduce0("DefList", @$.first_line);}
		;
Def		: Specifier DecList SEMI	{$$ = (char *)reduce(5, "Def", @$.first_line, $1, $2, $3);}
		| error SEMI %prec DEF_ERR	{$$ = (char *)reduce0("DefList", @$.first_line); meetError(); printf("Expect TYPE or ID\n");}
		;
DecList		: Dec				{$$ = (char *)reduce(3, "DecList", @$.first_line, $1);}
		| Dec COMMA DecList		{$$ = (char *)reduce(5, "DecList", @$.first_line, $1, $2, $3);}
		;
Dec		: VarDec			{$$ = (char *)reduce(3, "Dec", @$.first_line, $1);}
		| VarDec ASSIGNOP Exp		{$$ = (char *)reduce(5, "Dec", @$.first_line, $1, $2, $3);}
		;

Exp		: Exp ASSIGNOP Exp		{$$ = (char *)reduce(5, "Exp", @$.first_line, $1, $2, $3);}
		| Exp AND Exp			{$$ = (char *)reduce(5, "Exp", @$.first_line, $1, $2, $3);}
		| Exp OR Exp			{$$ = (char *)reduce(5, "Exp", @$.first_line, $1, $2, $3);}
		| Exp RELOP Exp			{$$ = (char *)reduce(5, "Exp", @$.first_line, $1, $2, $3);}
		| Exp PLUS Exp			{$$ = (char *)reduce(5, "Exp", @$.first_line, $1, $2, $3);}	
		| Exp MINUS Exp			{$$ = (char *)reduce(5, "Exp", @$.first_line, $1, $2, $3);}
		| Exp STAR Exp			{$$ = (char *)reduce(5, "Exp", @$.first_line, $1, $2, $3);}
		| Exp DIV Exp			{$$ = (char *)reduce(5, "Exp", @$.first_line, $1, $2, $3);}
		| LP Exp RP %prec LL_THAN_ELSE	{$$ = (char *)reduce(5, "Exp", @$.first_line, $1, $2, $3);}
		| MINUS Exp %prec HIGH_MINUS	{$$ = (char *)reduce(4, "Exp", @$.first_line, $1, $2);}
		| NOT Exp			{$$ = (char *)reduce(4, "Exp", @$.first_line, $1, $2);}
		| ID LP Args RP			{$$ = (char *)reduce(6, "Exp", @$.first_line, $1, $2, $3, $4);}
		| Exp LB Exp RB			{$$ = (char *)reduce(6, "Exp", @$.first_line, $1, $2, $3, $4);}
		| Exp DOT ID			{$$ = (char *)reduce(5, "Exp", @$.first_line, $1, $2, $3);}
		| ID				{$$ = (char *)reduce(3, "Exp", @$.first_line, $1);}
		| INT				{$$ = (char *)reduce(3, "Exp", @$.first_line, $1);}
		| FLOAT				{$$ = (char *)reduce(3, "Exp", @$.first_line, $1);}
		| error RP %prec RP_ERR		{$$ = (char *)reduce0("Exp", @$.first_line);meetError(); printf("Expect expression before ')'\n");}
		| Exp error			{$$ = (char *)reduce0("Exp", @$.first_line); meetError(); printf("expression error\n");}
		;

Args		: Exp COMMA Args		{$$ = (char *)reduce(5, "Args", @$.first_line, $1, $2, $3);}
		| Exp				{$$ = (char *)reduce(3, "Args", @$.first_line, $1);}
		| /**/				{$$ = (char *)reduce0("Args", @$.first_line);}
		;

%%
yyerror(char *msg){
	fprintf(stderr, "Error type B at Line %d: %s\n", yylineno, msg);
}
// len name line, $1...
Leaf *reduce(int len, ...){
	va_list ap;
	va_start(ap, len);

	char *name = va_arg(ap, char *);
	int line = va_arg(ap, int);
	Leaf *first = va_arg(ap, Leaf *);

	// 非终结符的valno=-1
	Leaf *tmp = makeLeaf(line, -1, 0, name, first->val);
	addTree(tmp);
	addChild(tmp, first);
	delTree(first);
	int i = 4;
	for (; i <= len; i ++) {
		first = va_arg(ap, Leaf *);
		addChild(tmp, first);
		delTree(first);
	}

	va_end(ap);
	return tmp;
}
Leaf *reduce0(char *name, int line){
	Value v;
	v.val_int = 0;
	v.val_double = 0.0;
	v.val_float = 0.0;
	memset(v.val_name, 0, 32);

	// 非终结符的valno=-1
	Leaf *tmp = makeLeaf(line, -1, 0, name, v);
	addTree(tmp);
	return tmp;
}

