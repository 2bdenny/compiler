%{
#include<sys/types.h>
#include"lex.yy.c"
#include"GrammarTree.h"
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
Program		: ExtDefList			
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

Specifier	: TYPE				
		| StructSpecifier		
		;
StructSpecifier	: STRUCT OptTag LC DefList RC	
		| STRUCT Tag			
		;
OptTag		: /* empty */			
		| ID				
		;
Tag		: ID				
		;

VarDec		: ID				
		| VarDec LB INT RB		
		;
FunDec		: ID LP VarList RP		
		| ID LP RP			
		;
VarList		: ParamDec COMMA VarList	
		| ParamDec			
		;
ParamDec	: Specifier VarDec		
		;

CompSt		: LC DefList StmtList RC	
		;
StmtList	: /* empty */			
		| Stmt StmtList			
		;
Stmt		: Exp SEMI			
		| CompSt			
		| RETURN Exp SEMI		
		| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
		| IF LP Exp RP Stmt ELSE Stmt
		| WHILE LP Exp RP Stmt		
		;

DefList		: /* empty */			
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

Exp		: Exp ASSIGNOP Exp		
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

