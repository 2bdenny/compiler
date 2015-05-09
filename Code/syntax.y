%{
#define YYSTYPE char*
#define YYDEBUG 1
#include <stdarg.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include "lex.yy.c"
#include "GrammarTree.h"
#define print_pos() printf("[%s %s %d]\n", __FILE__, __FUNCTION__, __LINE__)
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
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT HIGH_MINUS
%left LP RP LB RB DOT
%%
Program		: ExtDefList			{}
		;

ExtDefList	: ExtDef ExtDefList		{}
		| /* empty */			{}
		;

ExtDef		: Specifier ExtDecList{
			  Item *trace = (Item *)$2;
			  Item *item = trace;
			  while (trace != NULL){
				  item->type = ((Item *)$1)->type;
//				  printf("typename=%s\n", ((Item *)$1)->type_name);
				  cpy(item->type_name, ((Item *)$1)->type_name);
				  //print_pos();
				  trace = trace->next;
				  insertTable(item);
				  item = trace;
			  }
		  }SEMI{}
		| Specifier SEMI{}
		| Specifier FunDec {
			$$ = $2;
			//printf("FunDec line:%d\n", ((Item *)$2)->line);
			((Item *)$$)->ret_type = ((Item *)$1)->type;
			cpy(((Item *)$$)->ret_type_name, ((Item *)$1)->type_name);
			insertTable((Item *)$$);
			setScope((Item *)$$);
		  } CompSt{
			setScope(NULL);
			int t1 = getType($1);
			//printf("$3 type=%d, result_type=%d\n", ((Item *)$3)->type, ((Item *)$3)->result_type);
			int t2 = ((Item *)$3)->result_type;
			//printf("expect type:%d, return type:%d\n", t1, t2);
			if (!((t1 == TYPE_VAR_INT && (t2 == TYPE_VAR_INT || t2 == TYPE_INT)) || (t1 == TYPE_VAR_FLOAT && (t2 == TYPE_VAR_FLOAT || t2 == TYPE_FLOAT)) || (t1 == t2)))
				printf("Error type 8 at Line %d: return type not match\n", ((Item *)$2)->line);

		  }
		| error SEMI %prec EDF_ERR	{}
		;

ExtDecList	: VarDec{
			  $$ = $1;
		  }
		| VarDec COMMA ExtDecList	{
			$$ = $1;
			((Item *)$$)->next = (Item *)$3;
		  }
		;

Specifier	: TYPE {  // 符号表建立相关
			  $$ = (char *)newItem();
			  if (cmp(((Leaf *)$1)->val.val_name, "int") == 0) ((Item *)$$)->type = TYPE_VAR_INT;
			  else if (cmp(((Leaf *)$1)->val.val_name, "float") == 0) ((Item *)$$)->type = TYPE_VAR_FLOAT;
			  ((Item *)$$)->scope = getScope();
		  }
		| StructSpecifier {
			$$ = $1;
			((Item *)$$)->type = TYPE_VAR_STRUCT;
		  }
		;

StructSpecifier	: STRUCT OptTag LC{
			  $3 = $2;
			  if (((Item *)$3)->line == -1) ((Item *)$3)->line = ((Leaf *)$2)->line;
			  ((Item *)$3)->type = TYPE_STRUCT;
			  ((Item *)$3)->scope = getScope();
			  insertTable((Item *)$3);
			  setScope((Item *)$3);
		  } DefList RC {
			  $$ = (char *)newItem();
			  memcpy($$, $2, sizeof(Item));
			  if (((Item *)$$)->line == -1) ((Item *)$$)->line = ((Leaf *)$2)->line;
			  ((Item *)$$)->type = TYPE_VAR_STRUCT;

			  setScope(getScope()->scope);
			  ((Item *)$$)->scope = getScope();
		  }
		| STRUCT Tag {
			$$ = (char *)newItem();
			((Item *)$$)->type = TYPE_VAR_STRUCT;
			cpy(((Item *)$$)->type_name,((Item *)$2)->type_name);
			((Item *)$$)->scope = getScope();
		}
		| error RC %prec STR_ERR{}
		;

OptTag		: ID{
			  $$ = (char *)newItem();
			  if ($1 != NULL){
				  cpy(((Item *)$$)->type_name, ((Leaf *)$1)->val.val_name);
				  ((Item *)$$)->line = ((Leaf *)$1)->line;
			  }
		  }
		| /* empty */{
			$$ = (char *)newItem();
		  }
		;

Tag		: ID{
			  $$ = (char *)newItem();
			  if ($1 != NULL){
				  cpy(((Item *)$$)->type_name, ((Leaf *)$1)->val.val_name);
				  ((Item *)$$)->line = ((Leaf *)$1)->line;
			  }
		  }
		;

VarDec		: ID {
			  $$ = (char *)newItem();
			  ((Item *)$$)->scope = getScope();
			  cpy(((Item *)$$)->name, ((Leaf *)$1)->val.val_name);
			  ((Item *)$$)->line = ((Leaf *)$1)->line;
			  ((Item *)$$)->dimension = 0;
		  }
		| VarDec LB INT {
			$$ = $1;
			((Item *)$$)->dimension ++;
			int *tmp = ((Item *)$$)->dim_max;
			((Item *)$$)->dim_max = (int *)malloc((((Item *)$$)->dimension)*sizeof(int));
			int i;
			for (i = 0; i < ((Item *)$$)->dimension-1; i++) ((Item *)$$)->dim_max[i] = tmp[i];
			((Item *)$$)->dim_max[((Item *)$$)->dimension-1] = -1;
			free(tmp);
			((Item *)$$)->dim_max[((Item *)$$)->dimension-1] = ((Leaf *)$3)->val.val_int;
		  } RB{}
		| error RB %prec VDC_ERR{}
		;

FunDec		: ID LP {
			  $2 = (char *)newItem();
			  ((Item *)$2)->scope = NULL;
			  ((Item *)$2)->type = TYPE_FUNCTION;
			  //printf("fun line=%d\n",
			  ((Item *)$2)->line = ((Leaf *)$1)->line;
			  if ($1 != NULL) cpy(((Item *)$2)->name, ((Leaf *)$1)->val.val_name);
			  else printf("$1 is null\n");
			  setScope((Item *)$2);
		  } VarList RP{
			  $$ = $2;
			  setArgsNumber($2);
			  //printf("FunDec id line = %d\n", ((Leaf *)$1)->line);
			  ((Item *)$$)->line = ((Leaf *)$1)->line;
			  setScope(NULL);
		  }
		| ID LP RP{
			$$ = (char *)newItem();
			((Item *)$2)->args_num = 0;
			((Item *)$$)->scope = NULL;
			((Item *)$$)->type = TYPE_FUNCTION;
			((Item *)$$)->line = ((Leaf *)$1)->line;
			if ($1 != NULL) cpy(((Item *)$$)->name, ((Leaf *)$1)->val.val_name);
			else printf("$1 is null\n");
		  }
		| error RP %prec FUN_ERR	{}
		;

VarList		: ParamDec COMMA VarList {
			  ((Item *)$1)->next = (Item *)$3;
			  $$ = $1;
		  }
		| ParamDec {
			$$ = $1;
		 }
		;

ParamDec	: Specifier VarDec {
			  Item *trace = (Item *)$2;
			  Item *item = trace;
			  while (trace != NULL){
				  item->type = ((Item *)$1)->type;
				  cpy(item->type_name, ((Item *)$1)->type_name);
				  trace = trace->next;
				  insertTable(item);
				  item = trace;
			  }
		  }
		;

CompSt		: LC DefList StmtList RC{
			  if (getScope() != NULL)
			  	setScope((Item *)getScope()->scope);
			  if ($3 == NULL) $$ = (char *)newItem();
			  else {
				  $$ = (char *)newItem();
				  memcpy($$, $3, sizeof(Item));
				  //printf("compst -> $$ result type = %d\n", ((Item *)$$)->result_type);
			  }
		  }
		| error RC %prec COM_ERR	{}
		;

StmtList	: Stmt StmtList {
			  if ($2 == NULL) $$ = $1;
			  else $$ = $2;
			  //printf("$$ result type = %d\n", ((Item *)$$)->result_type);
		  }
		| /* empty */ { $$ = NULL; }
		;

Stmt		: Exp SEMI			{}
		| CompSt {$$ = $1;}
		| RETURN Exp SEMI {
			$$ = (char *)newItem();
			((Item *)$$)->result_type = ((Item *)$2)->type;
			cpy(((Item *)$$)->result_type_name, ((Item *)$2)->type_name);
		  }
		| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {}
		| IF LP Exp RP Stmt ELSE Stmt
		| WHILE LP Exp RP Stmt		{}
		| error SEMI %prec STM_ERR	{}
		;

DefList		: Def DefList			{}
		| /* empty */ 			{}
		;

Def		: Specifier DecList SEMI {
			  Item *trace = (Item *)$2;
			  Item *item = trace;
			  while (trace != NULL){
				  if (item->type != -1) {
					  if (item->type != ((Item *)$2)->type) 
						  printf("Error type 5 at Line %d: type not match of =\n", ((Item *)$2)->line);
				  }
				  item->type = ((Item *)$1)->type;
				  cpy(item->type_name, ((Item *)$1)->type_name);
				  trace = trace->next;
				  insertTable(item);
				  item = trace;
			  }
		  }
		| error SEMI %prec DEF_ERR	{}
		;

DecList		: Dec {
			  $$ = $1;
		  }
		| Dec COMMA DecList{
			((Item *)$1)->next = (Item *)$3;
			$$ = $1;
		  }
		;

Dec		: VarDec{
			  $$ = $1;
		  }
		| VarDec {
			$$ = $1;
		  }ASSIGNOP Exp {
			  $$ = $1;
			  ((Item *)$1)->type = getType($3);
			  cpy(((Item *)$1)->type_name, ((Item *)$3)->type_name);
		  }
		;

Exp		: Exp ASSIGNOP Exp {
			  if (getType($1) != getType($2)){
				  printf("Error type 5 at Line %d: type not match of =\n", ((Item *)$1)->line);
			  }
			  if (getType($1) == TYPE_INT || getType($1) == TYPE_FLOAT){
				  printf("Error type 6 at Line %d: only righter on the left of =\n", ((Item *)$1)->line);
			  }
			  $$ = $1;
		  }
		| Exp AND Exp {
			if ((getType($1) != TYPE_VAR_INT && getType($1) != TYPE_INT) || (getType($3) != TYPE_VAR_INT && getType($3) != TYPE_INT))
				printf("Error type 7 at Line %d: not match of %s\n", ((Leaf *)$2)->line, ((Leaf *)$2)->token);
			$$ = $1;
			int t1 = getType($1);
			int t2 = getType($3);
			((Item *)$$)->type = (t1 == TYPE_VAR_INT || t1 == TYPE_INT)? TYPE_INT:((t1 == TYPE_VAR_FLOAT || t1 == TYPE_FLOAT)? TYPE_FLOAT:-1);
		  }
		| Exp OR Exp {
			if ((getType($1) != TYPE_VAR_INT && getType($1) != TYPE_INT) || (getType($3) != TYPE_VAR_INT && getType($3) != TYPE_INT))
				printf("Error type 7 at Line %d: not match of %s\n", ((Leaf *)$2)->line, ((Leaf *)$2)->token);
			$$ = $1;
			int t1 = getType($1);
			int t2 = getType($3);
			((Item *)$$)->type = (t1 == TYPE_VAR_INT || t1 == TYPE_INT)? TYPE_INT:((t1 == TYPE_VAR_FLOAT || t1 == TYPE_FLOAT)? TYPE_FLOAT:-1);
		  }
		| Exp RELOP Exp {
			int t1 = getType($1);
			int t2 = getType($3);
			if (!(t1 == t2 && (t1 == TYPE_INT || t1 == TYPE_VAR_INT || t1 == TYPE_FLOAT || t1 == TYPE_VAR_FLOAT))){
				printf("Error type 7 at Line %d: not match of %s\n", ((Leaf *)$2)->line, ((Leaf *)$2)->token);
			}
			$$ = $1;
			((Item *)$$)->type = (t1 == TYPE_VAR_INT || t1 == TYPE_INT)? TYPE_INT:((t1 == TYPE_VAR_FLOAT || t1 == TYPE_FLOAT)? TYPE_FLOAT:-1);
		  }
		| Exp PLUS Exp {
			int t1 = getType($1);
			int t2 = getType($3);
			if (!(t1 == t2 && (t1 == TYPE_INT || t1 == TYPE_VAR_INT || t1 == TYPE_FLOAT || t1 == TYPE_VAR_FLOAT))){
				printf("Error type 7 at Line %d: not match of %s\n", ((Leaf *)$2)->line, ((Leaf *)$2)->token);
			}
			$$ = $1;
			((Item *)$$)->type = (t1 == TYPE_VAR_INT || t1 == TYPE_INT)? TYPE_INT:((t1 == TYPE_VAR_FLOAT || t1 == TYPE_FLOAT)? TYPE_FLOAT:-1);
		  }
		| Exp MINUS Exp {
			int t1 = getType($1);
			int t2 = getType($3);
			if (!(t1 == t2 && (t1 == TYPE_INT || t1 == TYPE_VAR_INT || t1 == TYPE_FLOAT || t1 == TYPE_VAR_FLOAT))){
				printf("Error type 7 at Line %d: not match of %s\n", ((Leaf *)$2)->line, ((Leaf *)$2)->token);
			}
			$$ = $1;
			((Item *)$$)->type = (t1 == TYPE_VAR_INT || t1 == TYPE_INT)? TYPE_INT:((t1 == TYPE_VAR_FLOAT || t1 == TYPE_FLOAT)? TYPE_FLOAT:-1);
		  }		
		| Exp STAR Exp {
			int t1 = getType($1);
			int t2 = getType($3);
			if (!(t1 == t2 && (t1 == TYPE_INT || t1 == TYPE_VAR_INT || t1 == TYPE_FLOAT || t1 == TYPE_VAR_FLOAT))){
				printf("Error type 7 at Line %d: not match of %s\n", ((Leaf *)$2)->line, ((Leaf *)$2)->token);
			}
			$$ = $1;
			((Item *)$$)->type = (t1 == TYPE_VAR_INT || t1 == TYPE_INT)? TYPE_INT:((t1 == TYPE_VAR_FLOAT || t1 == TYPE_FLOAT)? TYPE_FLOAT:-1);
		  }		
		| Exp DIV Exp {
			int t1 = getType($1);
			int t2 = getType($3);
			if (!(t1 == t2 && (t1 == TYPE_INT || t1 == TYPE_VAR_INT || t1 == TYPE_FLOAT || t1 == TYPE_VAR_FLOAT))){
				printf("Error type 7 at Line %d: not match of %s\n", ((Leaf *)$2)->line, ((Leaf *)$2)->token);
			}
			$$ = $1;
			((Item *)$$)->type = (t1 == TYPE_VAR_INT || t1 == TYPE_INT)? TYPE_INT:((t1 == TYPE_VAR_FLOAT || t1 == TYPE_FLOAT)? TYPE_FLOAT:-1);
		  }		
		| LP Exp RP %prec LL_THAN_ELSE	{}
		| MINUS Exp %prec HIGH_MINUS	{}
		| NOT Exp			{}
		| ID LP {
			$$ = (char *)getItem(((Leaf *)$1)->val.val_name);
			if (!isContain(((Leaf *)$1)->val.val_name)) {
				printf("Error type 2 at Line %d: function %s undefined\n", ((Leaf *)$1)->line, ((Leaf *)$1)->val.val_name);
			}
			else {
				if (((Item *)$$)->type != TYPE_FUNCTION)
					printf("Error type 11 at Line %d: %s not a function\n", ((Leaf *)$1)->line, ((Leaf *)$1)->val.val_name);
			}
	 	  }
		  Args RP {
			  Item *def_args = getArgs(((Leaf *)$1)->val.val_name);
			  Item *in_args = (Item *)$3;
			  if (!cmpArgs(def_args, in_args)){
				  printf("Error type 9 at Line %d: args not match of function %s\n", ((Leaf *)$1)->line, ((Leaf *)$1)->val.val_name);
			  }
		  }
		| Exp LB Exp RB			{}
		| Exp DOT ID			{}
		| ID { 
			if (!isContain(((Leaf *)$1)->val.val_name)) 
				printf("Error type 1 at Line %d: var %s undefined\n", ((Leaf *)$1)->line, ((Leaf *)$1)->val.val_name);
			$$ = (char *)getItem(((Leaf *)$1)->val.val_name);
		  }
		| INT {
			Item *tmp = newItem();
			tmp->line = ((Leaf *)$1)->line;
			tmp->type = TYPE_INT;
			$$ = (char *)tmp;
		  }
		| FLOAT {
			Item *tmp = newItem();
			tmp->line = ((Leaf *)$1)->line;
			tmp->type = TYPE_INT;
			$$ = (char *)tmp;
		  }
		| error RP %prec RP_ERR		{}
		| Exp error			{}
		;

Args		: Exp COMMA Args {
			Item *tmp = getItem(((Item *)$1)->name);
			$$ = (char *)newItem();
			memcpy($$, tmp, sizeof(Item));
			((Item *)$$)->next = (Item *)$3;
		  }
		| Exp {
			Item *tmp = getItem(((Item *)$1)->name);
			$$ = (char *)newItem();
			memcpy($$, tmp, sizeof(Item));
			((Item *)$$)->next = NULL;
		  }
		| /**/ {$$ = NULL;}
		;

%%
yyerror(char *msg){
	fprintf(stderr, "Error type B at Line %d: %s\n", yylineno, msg);
}

