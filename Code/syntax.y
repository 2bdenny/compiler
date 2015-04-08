%{
#include <sys/types.h>
#include "lex.yy.c"
#include "GrammarTree.h"
%}

/* declared types */
%token INT
%token FLOAT
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
%right NOT
%left LP RP LB RB DOT
%%
Program		: ExtDefList			{$$ = reduce1(@$.first_line, "Program", $1);}	/*High-level Definitions*/
		;
ExtDefList	: /* empty */			{$$ = reduce0(@$.first_line, "ExtDefList");}
		| ExtDef ExtDefList		{$$ = reduce2(@$.first_line, "ExtDefList", $1, $2);}	
		;
ExtDef		: Specifier ExtDecList SEMI	{$$ = reduce3(@$.first_line, "ExtDef", $1, $2, $3);}
		| Specifier SEMI		{$$ = reduce2(@$.first_line, "ExtDef", $1, $2);}
		| Specifier FunDec CompSt	{$$ = reduce3(@$.first_line, "ExtDef", $1, $2, $3);}
		;
ExtDecList	: VarDec			{$$ = reduce1(@$.first_line, "ExtDecList", $1);}
		| VarDec COMMA ExtDecList	{$$ = reduce3(@$.first_line, "ExtDecList", $1, $2, $3);}
		;

Specifier	: TYPE				{$$ = reduce1(@$.first_line, "Specifier", $1);}		/*Specifiers*/
		| StructSpecifier		{$$ = reduce1(@$.first_line, "Specifier", $1);}
		;
StructSpecifier	: STRUCT OptTag LC DefList RC	{$$ = reduce4(@$.first_line, "StructSpecifier", $1, $2, $3, $4);}
		| STRUCT Tag			{$$ = reduce2(@$.first_line, "StructSpecifier", $1, $2);}
		;
OptTag		: /* empty */			{$$ = reduce0(@$.first_line, "OptTag");}
		| ID				{$$ = reduce1(@$.first_line, "OptTag", $1);}
		;
Tag		: ID				{$$ = reduce1(@$.first_line, "Tag", $1);}
		;

VarDec		: ID				{$$ = reduce1(@$.first_line, "VarDec", $1);}	/*Declarators*/
		| VarDec LB INT RB		{$$ = reduce4(@$.first_line, "VarDec", $1, $2, $3, $4);}
		;
FunDec		: ID LP VarList RP		{$$ = reduce4(@$.first_line, "FunDec", $1, $2, $3, $4);}
		| ID LP RP			{$$ = reduce3(@$.first_line, "FunDec", $1, $2, $3);}
		;
VarList		: ParamDec COMMA VarList	{$$ = reduce3(@$.first_line, "VarList", $1, $2, $3);}
		| ParamDec			{$$ = reduce1(@$.first_line, "ParamDec", $1);}
		;
ParamDec	: Specifier VarDec		{$$ = reduce2(@$.first_line, "ParamDec", $1, $2);}
		;

CompSt		: LC DefList StmtList RC	{$$ = reduce4(@$.first_line, "ComptSt", $1, $2, $3, $4);}/*Statements*/
		;
StmtList	: /* empty */			{$$ = reduce0(@$.first_line, "StmtList");}
		| Stmt StmtList			{$$ = reduce2(@$.first_line, "StmtList", $1, $2);}
		;
Stmt		: Exp SEMI			{$$ = reduce2(@$.first_line, "Stmt", $1, $2);}
		| CompSt			{$$ = reduce1(@$.first_line, "Stmt", $1);}
		| RETURN Exp SEMI		{$$ = reduce3(@$.first_line, "Stmt", $1, $2, $3);}
		| IF LP Exp RP Stmt		{$$ = reduce5(@$.first_line, "Stmt", $1, $2, $3, $4, $5);}%prec LOWER_THAN_ELSE	
		| IF LP Exp RP Stmt ELSE Stmt	{$$ = reduce6(@$.first_line, "Stmt", $1, $2, $3, $4, $5, $6);}
		| WHILE LP Exp RP Stmt		{$$ = reduce5(@$.first_line, "Stmt", $1, $2, $3, $4, $5);}
		;

DefList		: /* empty */			{$$ = reduce0(@$.first_line, "DefList");}	/*Local Definitions*/
		| Def DefList			{$$ = reduce2(@$.first_line, "DefList", $1, $2);}
		;
Def		: Specifier DecList SEMI	{$$ = reduce3(@$.first_line, "Def", $1, $2, $3);}
		;
DecList		: Dec				{$$ = reduce1(@$.first_line, "DecList", $1);}
		| Dec COMMA DecList		{$$ = reduce3(@$.first_line, "DecList", $1, $2, $3);}
		;
Dec		: VarDec			{$$ = reduce1(@$.first_line, "Dec", $1);}
		| VarDec ASSIGNOP Exp		{$$ = reduce3(@$.first_line, "Dec", $1, $2, $3);}
		;

Exp		: Exp ASSIGNOP Exp		{$$ = reduce3(@$.first_line, "Exp", $1, $2, $3);}/*Expressions*/
		| Exp AND Exp			{$$ = reduce3(@$.first_line, "Exp", $1, $2, $3);}
		| Exp OR Exp			{$$ = reduce3(@$.first_line, "Exp", $1, $2, $3);}
		| Exp RELOP Exp			{$$ = reduce3(@$.first_line, "Exp", $1, $2, $3);}
		| Exp PLUS Exp			{$$ = reduce3(@$.first_line, "Exp", $1, $2, $3);}	
		| Exp MINUS Exp			{$$ = reduce3(@$.first_line, "Exp", $1, $2, $3);}
		| Exp STAR Exp			{$$ = reduce3(@$.first_line, "Exp", $1, $2, $3);}
		| Exp DIV Exp			{$$ = reduce3(@$.first_line, "Exp", $1, $2, $3);}
		| LP Exp RP			{$$ = reduce3(@$.first_line, "Exp", $1, $2, $3);}
		| MINUS Exp			{$$ = reduce2(@$.first_line, "Exp", $1, $2);}
		| NOT Exp			{$$ = reduce2(@$.first_line, "Exp", $1, $2);}
		| ID LP Args RP			{$$ = reduce4(@$.first_line, "Exp", $1, $2, $3, $4);}
		| Exp LB Exp RB			{$$ = reduce4(@$.first_line, "Exp", $1, $2, $3, $4);}
		| Exp DOT ID			{$$ = reduce3(@$.first_line, "Exp", $1, $2, $3);}
		| ID				{$$ = reduce1(@$.first_line, "Exp", $1);}
		| INT				{$$ = reduce1(@$.first_line, "Exp", $1);}
		| FLOAT				{$$ = reduce1(@$.first_line, "Exp", $1);}
		;
Args		: Exp COMMA Args		{$$ = reduce3(@$.first_line, "Args", $1, $2, $3);}
		| Exp				{$$ = reduce1(@$.first_line, "Args", $1);}
		;
%%
yyerror(char *msg){
	fprintf(stderr, "error: %s\n", msg);
}

