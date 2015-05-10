%{
#define YYSTYPE char*
#define YYDEBUG 1
#include <stdarg.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include "lex.yy.c"
#include "GrammarTree.h"
#define print_pos() printf("[%s %s %d]\n", __FILE__, __FUNCTION__, __LINE__)	//这个宏定义学自余子洵
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
			  // 全局变量的定义
			  Item *trace = (Item *)$2;
			  Item *item = trace;
			  while (trace != NULL){
				  item->type = ((Item *)$1)->type;
				  cpy(item->type_name, ((Item *)$1)->type_name);
				  trace = trace->next;
				  insertTable(item);
				  item = trace;
			  }
		  }SEMI{}
		| Specifier SEMI {}
		| Specifier FunDec {
			// 函数的定义
			$$ = $2;
			((Item *)$$)->ret_type = ((Item *)$1)->type;
			cpy(((Item *)$$)->ret_type_name, ((Item *)$1)->type_name);
//			insertTable((Item *)$$);
			setScope((Item *)$$);
		  } CompSt {
			setScope(NULL);
			int t1 = getType($1);
			int t2 = ((Item *)$4)->result_type;
			if (!((t1 == TYPE_VAR_INT && t2 == TYPE_INT)||(t1 == TYPE_VAR_FLOAT && t2 == TYPE_FLOAT)||(t1 == t2)||(t1 == TYPE_STRUCT && t2 == TYPE_VAR_STRUCT)||(t2 == -1)))
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
			  // 结构体的定义
			  $3 = $2;
			  if (((Item *)$3)->line == -1) ((Item *)$3)->line = ((Leaf *)$2)->line;
			  ((Item *)$3)->type = TYPE_STRUCT;
			  ((Item *)$3)->scope = getScope();
			  insertTable((Item *)$3);
			  setScope((Item *)$3);
		  } DefList RC {
			  // 这里往后如果不是semi则开始结构体变量的定义
			  $$ = (char *)newItem();
			  memcpy($$, $2, sizeof(Item));
			  if (((Item *)$$)->line == -1) ((Item *)$$)->line = ((Leaf *)$2)->line;
			  ((Item *)$$)->type = TYPE_VAR_STRUCT;

			  setScope(getScope()->scope);
			  ((Item *)$$)->scope = getScope();
		  }
		| STRUCT Tag {
			// 结构体变量的定义
			$$ = (char *)newItem();
			((Item *)$$)->type = TYPE_VAR_STRUCT;
			cpy(((Item *)$$)->type_name,((Item *)$2)->type_name);
			((Item *)$$)->scope = getScope();

			Item *exister = getItem(((Item *)$$)->type_name);
			if (exister == NULL){
				printf("Error type 17 at Line %d: struct %s undefined\n", ((Leaf *)$1)->line, ((Item *)$$)->type_name);
			}
		}
		| error RC %prec STR_ERR{}
		;

OptTag		: ID{
			  // 结构体名
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
			  // 结构体名
			  $$ = (char *)newItem();
			  if ($1 != NULL){
				  cpy(((Item *)$$)->type_name, ((Leaf *)$1)->val.val_name);
				  ((Item *)$$)->line = ((Leaf *)$1)->line;
			  }
		  }
		;

VarDec		: ID {
			  // 变量
			  $$ = (char *)newItem();
			  ((Item *)$$)->scope = getScope();
			  cpy(((Item *)$$)->name, ((Leaf *)$1)->val.val_name);
			  ((Item *)$$)->line = ((Leaf *)$1)->line;
			  ((Item *)$$)->dimension = 0;
		  }
		| VarDec LB INT {
			// 变量数组
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
			  // 带参函数
			  $2 = (char *)newItem();
			  ((Item *)$2)->scope = NULL;
			  ((Item *)$2)->type = TYPE_FUNCTION;
			  //printf("fun line=%d\n",
			  ((Item *)$2)->line = ((Leaf *)$1)->line;
			  if ($1 != NULL) cpy(((Item *)$2)->name, ((Leaf *)$1)->val.val_name);
			  else printf("$1 is null\n");
			  insertTable((Item *)$2);
			  setScope((Item *)$2);
		  } VarList RP{
			  $$ = $2;
			  setArgsNumber($2);
			  //printf("FunDec id line = %d\n", ((Leaf *)$1)->line);
			  ((Item *)$$)->line = ((Leaf *)$1)->line;
			  setScope(NULL);
		  }
		| ID LP RP{
			// 无参函数
			$$ = (char *)newItem();
			((Item *)$$)->args_num = 0;
			((Item *)$$)->scope = NULL;
			((Item *)$$)->type = TYPE_FUNCTION;
			((Item *)$$)->line = ((Leaf *)$1)->line;
			if ($1 != NULL) cpy(((Item *)$$)->name, ((Leaf *)$1)->val.val_name);
			else printf("$1 is null\n");
			insertTable((Item *)$$);
		  }
		| error RP %prec FUN_ERR	{}
		;

VarList		: ParamDec COMMA VarList {
			  // 参数列表定义
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
			  // 语句块定义
			  if (getScope() != NULL)
			  	setScope((Item *)getScope()->scope);
			  if ($3 == NULL) $$ = (char *)newItem();
			  else {
				  $$ = (char *)newItem();
				  memcpy($$, $3, sizeof(Item));
//				  printf("compst -> $$ result type = %d\n", ((Item *)$$)->result_type);
			  }
		  }
		| error RC %prec COM_ERR	{}
		;

StmtList	: Stmt StmtList {
			  if ($2 == NULL) $$ = $1;
			  else $$ = $2;
//			  printf("$$ result type = %d\n", ((Item *)$$)->result_type);
		  }
		| /* empty */ { $$ = NULL; }
		;

Stmt		: Exp SEMI {
			  $$ = $1;
		  }
		| CompSt {$$ = $1;}
		| RETURN Exp SEMI {
			$$ = $2;
			Item *t0 = (Item *)$$;
			Item *t2 = (Item *)$2;
			t0->result_type = t2->type;
			cpy(t0->result_type_name, t2->type_name);
//			if ($$ != NULL) printf("return result_type = %d\n", ((Item *)$$)->result_type);
		  }
		| IF LP Exp RP {
			Item *cond = (Item *)$3;
			if (cond->type != TYPE_INT && cond->type != TYPE_VAR_INT)
				printf("Error type ? at Line %d: error condition type\n", ((Leaf *)$1)->line);

			Item *tmp = newItem();
			tmp->line = ((Leaf *)$1)->line;
			tmp->type = TYPE_IF;
			insertTable(tmp);
			setScope(tmp);
		  }Stmt %prec LOWER_THAN_ELSE {
			if (getScope() != NULL) setScope(getScope()->scope);
			else setScope(NULL);
		  }
		| IF LP Exp RP {
			Item *cond = (Item *)$3;
			if (cond->type != TYPE_INT && cond->type != TYPE_VAR_INT)
				printf("Error type ? at Line %d: error condition type\n", ((Leaf *)$1)->line);
			Item *tmp = newItem();
			tmp->line = ((Leaf *)$1)->line;
			tmp->type = TYPE_IF;
			insertTable(tmp);
			setScope(tmp);
		  }Stmt {
			if (getScope() != NULL) setScope(getScope()->scope);
			else setScope(NULL);
		  }ELSE {
			Item *tmp = newItem();
			tmp->line = ((Leaf *)$6)->line;
			tmp->type = TYPE_ELSE;
			insertTable(tmp);
			setScope(tmp);
		  }Stmt{
			if (getScope() != NULL) setScope(getScope()->scope);
			else setScope(NULL);
		  }
		| WHILE LP Exp RP{
			Item *cond = (Item *)$3;
			if (cond->type != TYPE_INT && cond->type != TYPE_VAR_INT)
				printf("Error type ? at Line %d: error condition type\n", ((Leaf *)$1)->line);
			Item *tmp = newItem();
			tmp->line = ((Leaf *)$1)->line;
			tmp->type = TYPE_WHILE;
			insertTable(tmp);
			setScope(tmp);
		  }Stmt{
			if (getScope() != NULL) setScope(getScope()->scope);
			else setScope(NULL);
		  }
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
					  else if (item->type == TYPE_VAR_STRUCT){
						  Item *list1 = getStructMember((char *)getItem(item->type_name));
						  Item *list2 = getStructMember((char *)getItem(((Item *)$2)->type_name));
						  if (!cmpArgs(list1, list2)) 
							  printf("Error type 5 at Line %d: structure type not match of =\n", ((Item *)$2)->line);
					  }
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
			  if (getScope() != NULL && (getScope()->type == TYPE_STRUCT || getScope()->type == TYPE_VAR_STRUCT)){
				  printf("Error type 15 at Line %d: can not = in struct\n", ((Item *)$1)->line);
			  }
			  $$ = $1;
			  ((Item *)$1)->type = getType($3);
			  cpy(((Item *)$1)->type_name, ((Item *)$3)->type_name);
			  ((Item *)$1)->scope = getScope();
		  }
		;

Exp		: Exp ASSIGNOP Exp {
			  int t1 = getType($1);
			  int t2 = getType($3);
//			  printf("t1 = %d, t2 = %d\n", t1, t2);
			  if (getType($1) == TYPE_INT || getType($1) == TYPE_FLOAT){
				  printf("Error type 6 at Line %d: only righter on the left of =\n", ((Item *)$1)->line);
			  }
			  else if ((t1 == TYPE_VAR_INT && t2 == TYPE_INT)||(t1 == TYPE_VAR_FLOAT && t2 == TYPE_FLOAT));
			  else if (getType($1) != getType($3) && t1 != -1){
				  printf("Error type 5 at Line %d: type not match of =\n", ((Leaf *)$2)->line);
			  }
			  else if (getType($1) == getType($3) && getType($1) == TYPE_VAR_STRUCT){
				 Item *s1 = getItem(((Item *)$1)->type_name);
				 Item *s2 = getItem(((Item *)$3)->type_name);
				 Item *list1 = getStructMember((char *)s1);
				 Item *list2 = getStructMember((char *)s2);
				 if (cmpArgs(list1, list2) != true)
					 printf("Error type 5 at Line %d: structure not match of =\n", ((Leaf*)$2)->line);
			  }
			  $$ = $1;
//			  printf("$1 = %d, $2 = %d, $3 = %d\n", ((Item *)$1)->type, ((Item *)$2)->type, ((Item *)$3)->type);
		  }
		| Exp AND Exp {
			if ((getType($1) != TYPE_VAR_INT && getType($1) != TYPE_INT) || (getType($3) != TYPE_VAR_INT && getType($3) != TYPE_INT))
				printf("Error type 7 at Line %d: not match of %s\n", ((Leaf *)$2)->line, ((Leaf *)$2)->token);
			$$ = $1;
			int t1 = getType($1);
			int t2 = getType($3);
			((Item *)$$)->type = TYPE_INT;
		  }
		| Exp OR Exp {
			if ((getType($1) != TYPE_VAR_INT && getType($1) != TYPE_INT) || (getType($3) != TYPE_VAR_INT && getType($3) != TYPE_INT))
				printf("Error type 7 at Line %d: not match of %s\n", ((Leaf *)$2)->line, ((Leaf *)$2)->token);
			$$ = $1;
			int t1 = getType($1);
			int t2 = getType($3);
			((Item *)$$)->type = TYPE_INT;
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
//			printf("t1_type=%d, t2_type=%d\n", t1, t2);
			if (!(((t1 == TYPE_INT || t1 == TYPE_VAR_INT) && (t2 == TYPE_INT || t2 == TYPE_VAR_INT)) || 
					((t1 == TYPE_FLOAT || t1 == TYPE_VAR_FLOAT)&&(t2 == TYPE_FLOAT || t2 == TYPE_VAR_FLOAT)))){
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
			  Item *in_args = (Item *)$4;

			  if (!cmpArgs(def_args, in_args)){
				  printf("Error type 9 at Line %d: args not match of function %s\n", ((Leaf *)$1)->line, ((Leaf *)$1)->val.val_name);
			  }
//			  printf("%s\n", ((Item *)$4)->name);
		  }
		| Exp LB Exp RB	{
			Item *var = (Item *)$1;
			Item *var3 = (Item *)$3;
			if (var->type == TYPE_VAR_INT || var->type == TYPE_VAR_FLOAT || var->type == TYPE_VAR_STRUCT)
				if (var->dimension <= 0) printf("Error type 10 at Line %d: %s not array var\n", ((Leaf *)$2)->line, var->name);
				else if (var3->type != TYPE_INT && var3->type != TYPE_VAR_INT)
					printf("Error type 12 at Line %d: error type between [ ]\n", ((Leaf *)$2)->line);
		  }
		| Exp DOT ID {
			Item *exp = (Item *)$1;
			if (exp->type != TYPE_STRUCT || exp->type != TYPE_VAR_STRUCT) {
				printf("Error type 13 at Line %d: %s not a struct\n", ((Leaf *)$2)->line, exp->name);
			}
			else {
				Item *component = getItem(exp->name);
				if (component == NULL || component->scope != exp) {
					printf("Error type 14 at Line %d: %s not %s 's component\n", ((Leaf *)$2)->line, component->name, exp->type_name);
				}
			}
		  }
		| ID { 
			if (!isContain(((Leaf *)$1)->val.val_name)) 
				printf("Error type 1 at Line %d: var %s undefined\n", ((Leaf *)$1)->line, ((Leaf *)$1)->val.val_name);
			$$ = (char *)getItem(((Leaf *)$1)->val.val_name);
			if ($$ == NULL) $$ = (char *)newItem();

			((Item *)$$)->result_type = ((Item *)$$)->type;
			cpy(((Item *)$$)->result_type_name, ((Item *)$$)->type_name);
//			printf("exp type=%d\n", ((Item *)$$)->result_type);
		  }
		| INT {
			Item *tmp = newItem();
			tmp->line = ((Leaf *)$1)->line;
			tmp->type = TYPE_INT;
			$$ = (char *)tmp;
//			printf("exp type=%d\n", ((Item *)$$)->type);
		  }
		| FLOAT {
			Item *tmp = newItem();
			tmp->line = ((Leaf *)$1)->line;
			tmp->type = TYPE_FLOAT;
			$$ = (char *)tmp;
//			printf("exp type=%d\n", ((Item *)$$)->type);
		  }
		| error RP %prec RP_ERR		{}
		| Exp error			{}
		;

Args		: Exp COMMA Args {
			$$ = (char *)newItem();
			memcpy($$, $1, sizeof(Item));
			((Item *)$$)->next = (Item *)$3;
//			displayTable((Item *)$$);
		  }
		| Exp {
			$$ = (char *)newItem();
			memcpy($$, $1, sizeof(Item));
			((Item *)$$)->next = NULL;
//			displayTable((Item *)$$);
		  }
		| /**/ {$$ = NULL;}
		;

%%
yyerror(char *msg){
	fprintf(stderr, "Error type B at Line %d: %s\n", yylineno, msg);
}

