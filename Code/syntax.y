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
%right MINUS NOT
%left LP RP LB RB DOT
%%
Program		: ExtDefList	{reduce1("Program");}	/*High-level Definitions*/
		;
ExtDefList	: /* empty */	{reduce0("ExtDefList");}
		| ExtDef ExtDefList	{reduce2("ExtDefList");}	
		;
ExtDef		: Specifier ExtDecList SEMI	{reduce3("ExtDef");}
		| Specifier SEMI		{reduce2("ExtDef");}
		| Specifier FunDec CompSt	{reduce3("ExtDef");}
		;
ExtDecList	: VarDec	{reduce1("ExtDecList");}
		| VarDec COMMA ExtDecList	{reduce3("ExtDecList");}
		;

Specifier	: TYPE		{reduce1("Specifier");}		/*Specifiers*/
		| StructSpecifier	{reduce1("Specifier");}
		;
StructSpecifier	: STRUCT OptTag LC DefList RC	{reduce4("StructSpecifier");}
		| STRUCT Tag	{reduce2("StructSpecifier");}
		;
OptTag		: /* empty */	{reduce0("OptTag");}
		| ID		{reduce1("OptTag");}
		;
Tag		: ID		{reduce1("Tag");}
		;

VarDec		: ID		{reduce1("VarDec");}	/*Declarators*/
		| VarDec LB INT RB	{reduce4("VarDec");}
		;
FunDec		: ID LP VarList RP	{reduce4("FunDec");}
		| ID LP RP	{reduce3("FunDec");}
		;
VarList		: ParamDec COMMA VarList	{reduce3("VarList");}
		| ParamDec	{reduce1("ParamDec");}
		;

ComptSt		: LC DefList StmtList RC	{reduce4("ComptSt");}/*Statements*/
		;
StmtList	: /* empty */	{reduce0("StmtList");}
		| Stmt StmtList	{reduce2("StmtList");}
		;
Stmt		: Exp SEMI	{reduce2("Stmt");}
		| CompSt	{reduce1("Stmt");}
		| RETURN Exp SEMI	{reduce3("Stmt");}
		| IF LP Exp RP Stmt	%prec LOWER_THAN_ELSE	{reduce5("Stmt");}
		| IF LP Exp RP Stmt ELSE Stmt	{reduce6("Stmt");}
		| WHILE LP Exp RP Stmt	{reduce5("Stmt");}
		;

DefList		: /* empty */	{reduce0("DefList");}	/*Local Definitions*/
		| Def DefList	{reduce2("DefList");}
		;
Def		: Specifier DecList SEMI	{reduce3("Def");}
		;
DecList		: Dec		{reduce1("DecList");}
		| Dec COMMA DecList	{reduce3("DecList");}
		;
Dec		: VarDec	{reduce1("Dec");}
		| VarDec ASSIGNOP Exp	{reduce3("Dec");}
		;

Exp		: Exp ASSIGNOP Exp	{reduce3("Exp");}/*Expressions*/
		| Exp AND Exp		{reduce3("Exp");}
		| Exp OR Exp		{reduce3("Exp");}
		| Exp RELOP Exp		{reduce3("Exp");}
		| Exp PLUS Exp		{reduce3("Exp");}	
		| Exp MINUS Exp	{reduce3("Exp");}
		| Exp STAR Exp	{reduce3("Exp");}
		| Exp DIV Exp	{reduce3("Exp");}
		| LP Exp RP	{reduce3("Exp");}
		| MINUS Exp	{reduce2("Exp");}
		| NOT Exp	{reduce2("Exp");}
		| ID LP Args RP	{reduce4("Exp");}
		| Exp LB Exp RB	{reduce4("Exp");}
		| Exp DOT ID	{reduce3("Exp");}
		| ID		{reduce1("Exp");}
		| INT		{reduce1("Exp");}
		| FLOAT		{reduce1("Exp");}
		;
Args		: Exp COMMA Args{reduce3("Args");}
		| Exp		{reduce1("Args");}
		;
%%
yyerror(char *msg){
	fprintf(stderr, "error: %s\n", msg);
}
void reduce1(char *name){
	$$ = (int)makeLeaf(@$.first_line, ((Leaf *)$1)->valno, 0, name, ((Leaf *)$1)->val);
	addChild((Leaf *)$$, (Leaf *)$1);

	addTree((Leaf *)$$);
	delTree((Leaf *)$1);
}
void reduce0(char *name){
	$$ = (int)makeLeaf(@$.first_line, ((Leaf *)$1)->valno, 0, name, ((Leaf *)$1)->val);
	addTree((Leaf *)$$);
}
void reduce2(char *name){
	$$ = (int)makeLeaf(@$.first_line, ((Leaf *)$1)->valno, 0, name, ((Leaf *)$1)->val);
	addChild((Leaf *)$$, (Leaf *)$1);
	addChild((Leaf *)$$, (Leaf *)$2);

	addTree((Leaf *)$$);
	delTree((Leaf *)$1);
	delTree((Leaf *)$2);
}
void reduce3(char *name){
	$$ = (int)makeLeaf(@$.first_line, ((Leaf *)$1)->valno, 0, name, ((Leaf *)$1)->val);
	addChild((Leaf *)$$, (Leaf *)$1);
	addChild((Leaf *)$$, (Leaf *)$2);
	addChild((Leaf *)$$, (Leaf *)$3);

	addTree((Leaf *)$$);
	delTree((Leaf *)$1);
	delTree((Leaf *)$2);
	delTree((Leaf *)$3);
}
void reduce4(char *name){
	$$ = (int)makeLeaf(@$.first_line, ((Leaf *)$1)->valno, 0, name, ((Leaf *)$1)->val);
	addChild((Leaf *)$$, (Leaf *)$1);
	addChild((Leaf *)$$, (Leaf *)$2);
	addChild((Leaf *)$$, (Leaf *)$3);
	addChild((Leaf *)$$, (Leaf *)$4);

	addTree((Leaf *)$$);
	delTree((Leaf *)$1);
	delTree((Leaf *)$2);
	delTree((Leaf *)$3);
	delTree((Leaf *)$4);
}
void reduce5(char *name){
	$$ = (int)makeLeaf(@$.first_line, ((Leaf *)$1)->valno, 0, name, ((Leaf *)$1)->val);
	addChild((Leaf *)$$, (Leaf *)$1);
	addChild((Leaf *)$$, (Leaf *)$2);
	addChild((Leaf *)$$, (Leaf *)$3);
	addChild((Leaf *)$$, (Leaf *)$4);
	addChild((Leaf *)$$, (Leaf *)$5);

	addTree((Leaf *)$$);
	delTree((Leaf *)$1);
	delTree((Leaf *)$2);
	delTree((Leaf *)$3);
	delTree((Leaf *)$4);
	delTree((Leaf *)$5);
}
void reduce6(char *name){
	$$ = (int)makeLeaf(@$.first_line, ((Leaf *)$1)->valno, 0, name, ((Leaf *)$1)->val);
	addChild((Leaf *)$$, (Leaf *)$1);
	addChild((Leaf *)$$, (Leaf *)$2);
	addChild((Leaf *)$$, (Leaf *)$3);
	addChild((Leaf *)$$, (Leaf *)$4);
	addChild((Leaf *)$$, (Leaf *)$5);
	addChild((Leaf *)$$, (Leaf *)$6);

	addTree((Leaf *)$$);
	delTree((Leaf *)$1);
	delTree((Leaf *)$2);
	delTree((Leaf *)$3);
	delTree((Leaf *)$4);
	delTree((Leaf *)$5);
	delTree((Leaf *)$6);
}
