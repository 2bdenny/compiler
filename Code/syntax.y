%{
#define YYSTYPE char*
#define YYDEBUG 1
#include <stdarg.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
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

			  // middle start
			  Item *var = (Item *)$1;
			  Item *tp = getTempType();
			  int arr_num = getArrayNum(var);
			  if (arr_num > 0) printf("DEC %s %d\n", var->name, getTypeSize(tp)*arr_num);
			  else if (NULL != tp && TYPE_VAR_STRUCT == tp->type) printf("DEC %s %d\n", var->name, getTypeSize(tp));
		  }
		| VarDec COMMA ExtDecList	{
			$$ = $1;
			((Item *)$$)->next = (Item *)$3;

			// middle start
			Item *var = (Item *)$1;
			Item *tp = getTempType();
			int arr_num = getArrayNum(var);
			if (arr_num > 0) printf("DEC %s %d\n", var->name, getTypeSize(tp)*arr_num);
			else if (NULL != tp && TYPE_VAR_STRUCT == tp->type) printf("DEC %s %d\n", var->name, getTypeSize(tp));
		  }
		;

Specifier	: TYPE {  // 符号表建立相关
			  $$ = (char *)newItem();
			  if (cmp(((Leaf *)$1)->val.val_name, "int") == 0) ((Item *)$$)->type = TYPE_VAR_INT;
			  else if (cmp(((Leaf *)$1)->val.val_name, "float") == 0) ((Item *)$$)->type = TYPE_VAR_FLOAT;
			  ((Item *)$$)->scope = getScope();
			  saveTempType((Item *)$$);
		  }
		| StructSpecifier {
			$$ = $1;
			((Item *)$$)->type = TYPE_VAR_STRUCT;
			saveTempType((Item *)$$);
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
			cpy(((Item *)$$)->type_name, getAnonymousStruct());
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

			  // middle start
			  printf("FUNCTION %s :\n", ((Leaf *)$1)->val.val_name);
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

			// middle start
			printf("FUNCTION %s :\n", ((Leaf *)$1)->val.val_name);
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
			  if (trace != NULL){
				  item->type = ((Item *)$1)->type;
				  cpy(item->type_name, ((Item *)$1)->type_name);
				  insertTable(item);

				  //middle start
				  printf("PARAM %s\n", item->name);
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
			  Item *t1 = (Item *)$1;

			  Item *trace = (Item *)$2;
			  Item *item = trace;
			  //printf("Def->Specifier DecList SEMI\n");
			  //displayTable((Item *)$2);
			  while (trace != NULL){
				  if (item->type != -1) {
					  if (item->type != t1->type){
						  if (TYPE_VAR_INT == t1->type && TYPE_INT == item->type){
						  }
						  else if (TYPE_VAR_FLOAT == t1->type && TYPE_FLOAT == item->type){
						  }
						  else printf("Error type 5 at Line %d: type not match of =\n", ((Item *)$2)->line);
					  }
					  else if (item->type == TYPE_VAR_STRUCT){
						  Item *list1 = getStructMember((char *)getItem(item->type_name));
						  Item *list2 = getStructMember((char *)getItem(((Item *)$2)->type_name));
						  if (!cmpArgs(list1, list2)) 
							  printf("Error type 5 at Line %d: structure type not match of =\n", ((Item *)$2)->line);
					  }
				  }
				  item->type = t1->type;
				  cpy(item->type_name, t1->type_name);
				  trace = trace->next;
//				  printf("Def insert %s\n", item->name);
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

			  // middle start
			  if (getScope() != NULL && (getScope()->type == TYPE_STRUCT || getScope()->type == TYPE_VAR_STRUCT));
			  else{
				  Item *tp = getTempType();
				  Item *var = (Item *)$1;
				  int arr_num = getArrayNum(var);
				  if (arr_num > 0) printf("DEC %s %d\n", var->name, arr_num*getTypeSize(tp));
				  else if (NULL != tp && TYPE_VAR_STRUCT == tp->type) printf("DEC %s %d\n", var->name, getTypeSize(tp));
			  }
		  }
		| VarDec {
			// middle start
			if (getScope() != NULL && (getScope()->type == TYPE_STRUCT || getScope()->type == TYPE_VAR_STRUCT));
			else{
				Item *tp = getTempType();
				Item *var = (Item *)$1;
				int arr_num = getArrayNum(var);
				if (arr_num > 0) printf("DEC %s %d\n", var->name, arr_num*getTypeSize(tp));
				else if (NULL != tp && TYPE_VAR_STRUCT == tp->type) printf("DEC %s %d\n", var->name, getTypeSize(tp));
			}
		  }ASSIGNOP Exp {
			  if (getScope() != NULL && (getScope()->type == TYPE_STRUCT || getScope()->type == TYPE_VAR_STRUCT)){
				  printf("Error type 15 at Line %d: can not = in struct\n", ((Item *)$1)->line);
			  }
			  Item *e1 = (Item *)$1;
			  Item *e2 = (Item *)$4;
			  if (TYPE_FUNCTION == e2->type) {
				  e1->type = e2->ret_type;
				  cpy(e1->type_name, e2->ret_type_name);
			  }
			  else {
			  	e1->type = e2->type;
				cpy(e1->type_name, e2->type_name);
			  }
			  e1->scope = getScope();

			  // middle start
			  Item *tp = getTempType();
			  if (TYPE_VAR_INT == tp->type || TYPE_VAR_FLOAT == tp->type){
				  printExp(&e2);
				  printf("%s := %s\n", e1->name, e2->name);
			  }
			  else if (TYPE_VAR_STRUCT == tp->type){
				  printExp(&e2);
				  int size = getTypeSize(tp);
				  int i = 0;
				  char *tvar1 = getTempVar();
				  char *tvar2 = getTempVar();
				  printf("%s := &%s\n", tvar1, e1->name);
				  printf("%s := &%s\n", tvar2, e2->name);
				  for (; i < size; i += 4){
					  printf("*%s := *%s\n", tvar1, tvar2);
					  if (i+4 < size){
						  printf("%s := %s + #4\n", tvar1, tvar1);
						  printf("%s := %s + #4\n", tvar2, tvar2);
					  }
				  }
			  }

			  $$ = (char *)e1;
		  }
		;

Exp		: Exp ASSIGNOP Exp {
			  Item *e1 = (Item *)$1;
			  Item *e2 = (Item *)$3;
			  int t1 = e1->type;
			  int t2 = e2->type;
			  if (t1 == TYPE_VAR_INT || t1 == TYPE_VAR_FLOAT || t1 == TYPE_VAR_STRUCT){
				  if (TYPE_VAR_INT == t1 && (TYPE_VAR_INT == t2 || TYPE_INT == t2)){
					  // middle start
					  printExp(&e1);
					  printExp(&e2);
					  printf("%s := %s\n", e1->name, e2->name);
				  }
				  else if (TYPE_VAR_FLOAT == t1 && (TYPE_VAR_FLOAT == t2 || TYPE_FLOAT == t2)){
					  // middle start
					  printExp(&e1);
					  printExp(&e2);
					  printf("%s := %s\n", e1->name, e2->name);
				  }
				  else if (TYPE_VAR_STRUCT == t1 && TYPE_VAR_STRUCT == t2){
					  Item *s1 = getItem(((Item *)$1)->type_name);
					  Item *s2 = getItem(((Item *)$3)->type_name);
					  Item *list1 = getStructMember((char *)s1);
					  Item *list2 = getStructMember((char *)s2);
					  if (cmpArgs(list1, list2) != true)
						  printf("Error type 5 at Line %d: structure not match of =\n", ((Leaf*)$2)->line);
					  else {
						  // middle start
						  printExp(&e1);
						  printExp(&e2);
						  int size = getTypeSize(e1);
						  int i = 0;
						  char *tvar1 = getTempVar();
						  char *tvar2 = getTempVar();
						  printf("%s := &%s\n", tvar1, e1->name);
						  printf("%s := &%s\n", tvar2, e2->name);
						  for (; i < size; i += 4){
							  printf("*%s := *%s\n", tvar1, tvar2);
							  if (i+4 < size){
								  printf("%s := %s + #4\n", tvar1, tvar1);
								  printf("%s := %s + #4\n", tvar2, tvar2);
							  }
						  }
					  }
				  }
				  else printf("Error type 5 at Line %d: type not match of =\n", ((Leaf *)$2)->line);
				  $$ = $1;
			  }
			  else {
				  printf("Error type 6 at Line %d: only righter on the left of =\n", e1->line);
				  e1->type = TYPE_VAR_INT;
				  $$ = (char *)e1;
			  }
		  }
		| Exp AND Exp {
			Item *e1 = (Item *)$1;
			Item *e2 = (Item *)$3;
			int t1 = e1->type;
			int t2 = e2->type;
			if ((TYPE_INT == t1 || TYPE_VAR_INT == t1) && (TYPE_INT == t2 || TYPE_VAR_INT == t2)){
				e1->type = TYPE_INT;
			}
			else {
				printf("Error type 7 at Line %d: not match of %s\n", ((Leaf *)$2)->line, ((Leaf *)$2)->token);
				e1->type = TYPE_INT;
			}

			//middle start
			printExp(&e1);
			printExp(&e2);
			char *dollar = getTempVar();
			printf("%s := %s && %s\n", dollar, e1->name, e2->name);
			cpy(e1->name, dollar);
			$$ = (char *)e1;
		  }
		| Exp OR Exp {
			Item *e1 = (Item *)$1;
			Item *e2 = (Item *)$3;
			int t1 = e1->type;
			int t2 = e2->type;
			if ((TYPE_INT == t1 || TYPE_VAR_INT == t1) && (TYPE_INT == t2 || TYPE_VAR_INT == t2)){
				e1->type = TYPE_INT;
			}
			else {
				printf("Error type 7 at Line %d: not match of %s\n", ((Leaf *)$2)->line, ((Leaf *)$2)->token);
				e1->type = TYPE_INT;
			}

			// middle start
			printExp(&e1);
			printExp(&e2);
			char *dollar = getTempVar();
			printf("%s := %s || %s\n", dollar, e1->name, e2->name);
			cpy(e1->name, dollar);
			$$ = (char *)e1;
		  }
		| Exp RELOP Exp {
			Item *e1 = (Item *)$1;
			Item *e2 = (Item *)$3;
			int t1 = e1->type;
			int t2 = e2->type;
			if ((TYPE_INT == t1 || TYPE_VAR_INT == t1) && (TYPE_INT == t2 || TYPE_VAR_INT == t2)){
				e1->type = TYPE_INT;
			}
			else {
				printf("Error type 7 at Line %d: not match of %s\n", ((Leaf *)$2)->line, ((Leaf *)$2)->token);
				e1->type = TYPE_INT;
			}

			//middle start
			printExp(&e1);
			printExp(&e2);
			char *dollar = getTempVar();
			printf("%s := %s %s %s\n", dollar, e1->name, ((Leaf *)$2)->val.val_name, e2->name);
			cpy(e1->name, dollar);
			$$ = (char *)e1;
		  }
		| Exp PLUS Exp {
			Item *e1 = (Item *)$1;
			Item *e2 = (Item *)$3;
			int t1 = e1->type;
			int t2 = e2->type;
			if ((TYPE_VAR_INT == t1 || TYPE_INT == t1) && (TYPE_VAR_INT == t2 || TYPE_INT == t2)){
				e1->type = TYPE_INT;
			}
			else if ((TYPE_VAR_FLOAT == t1 || TYPE_FLOAT == t1) && (TYPE_VAR_FLOAT == t2 || TYPE_FLOAT == t2)){
				e1->type = TYPE_FLOAT;
			}
			else {
				printf("Error type 7 at Line %d: not match of %s\n", ((Leaf *)$2)->line, ((Leaf *)$2)->token);
				e1->type = TYPE_INT;
			}

			//middle start
			printExp(&e1);
			printExp(&e2);
			char *dollar = getTempVar();
			printf("%s := %s + %s\n", dollar, e1->name, e2->name);
			cpy(e1->name, dollar);
			$$ = (char *)e1;
		  }
		| Exp MINUS Exp {
			Item *e1 = (Item *)$1;
			Item *e2 = (Item *)$3;
			int t1 = e1->type;
			int t2 = e2->type;
			if ((TYPE_VAR_INT == t1 || TYPE_INT == t1) && (TYPE_VAR_INT == t2 || TYPE_INT == t2)){
				e1->type = TYPE_INT;
			}
			else if ((TYPE_VAR_FLOAT == t1 || TYPE_FLOAT == t1) && (TYPE_VAR_FLOAT == t2 || TYPE_FLOAT == t2)){
				e1->type = TYPE_FLOAT;
			}
			else {
				printf("Error type 7 at Line %d: not match of %s\n", ((Leaf *)$2)->line, ((Leaf *)$2)->token);
				e1->type = TYPE_INT;
			}

			//middle start
			printExp(&e1);
			printExp(&e2);
			char *dollar = getTempVar();
			printf("%s := %s - %s\n", dollar, e1->name, e2->name);
			cpy(e1->name, dollar);
			$$ = (char *)e1;
		  }		
		| Exp STAR Exp {
			Item *e1 = (Item *)$1;
			Item *e2 = (Item *)$3;
			int t1 = e1->type;
			int t2 = e2->type;
			if ((TYPE_VAR_INT == t1 || TYPE_INT == t1) && (TYPE_VAR_INT == t2 || TYPE_INT == t2)){
				e1->type = TYPE_INT;
			}
			else if ((TYPE_VAR_FLOAT == t1 || TYPE_FLOAT == t1) && (TYPE_VAR_FLOAT == t2 || TYPE_FLOAT == t2)){
				e1->type = TYPE_FLOAT;
			}
			else {
				printf("Error type 7 at Line %d: not match of %s\n", ((Leaf *)$2)->line, ((Leaf *)$2)->token);
				e1->type = TYPE_INT;
			}

			//middle start
			printExp(&e1);
			printExp(&e2);
			char *dollar = getTempVar();
			printf("%s := %s * %s\n", dollar, e1->name, e2->name);
			cpy(e1->name, dollar);
			$$ = (char *)e1;
		  }		
		| Exp DIV Exp {
			Item *e1 = (Item *)$1;
			Item *e2 = (Item *)$3;
			int t1 = e1->type;
			int t2 = e2->type;
			if ((TYPE_VAR_INT == t1 || TYPE_INT == t1) && (TYPE_VAR_INT == t2 || TYPE_INT == t2)){
				e1->type = TYPE_INT;
			}
			else if ((TYPE_VAR_FLOAT == t1 || TYPE_FLOAT == t1) && (TYPE_VAR_FLOAT == t2 || TYPE_FLOAT == t2)){
				e1->type = TYPE_FLOAT;
			}
			else {
				printf("Error type 7 at Line %d: not match of %s\n", ((Leaf *)$2)->line, ((Leaf *)$2)->token);
				e1->type = TYPE_INT;
			}

			//middle start
			printExp(&e1);
			printExp(&e2);
			char *dollar = getTempVar();
			printf("%s := %s * %s\n", dollar, e1->name, e2->name);
			cpy(e1->name, dollar);
			$$ = (char *)e1;
		  }		
		| LP Exp RP %prec LL_THAN_ELSE	{$$ = $2;}
		| MINUS Exp %prec HIGH_MINUS {
			Item *e1 = (Item *)$2; 
			int t1 = e1->type;
			if (TYPE_INT == t1 || TYPE_VAR_INT == t1){
				e1->type = TYPE_INT;
			}
			else if (TYPE_FLOAT == t1 || TYPE_VAR_FLOAT == t1){
				e1->type = TYPE_FLOAT;
			}
			else {
				printf("Error type 7 at Line %d: not match of %s\n", ((Leaf *)$1)->line, ((Leaf *)$1)->token);
				e1->type = TYPE_INT;
			}

			// middle start
			printExp(&e1);
			char *dollar = getTempVar();
			printf("%s := #0 - %s\n", dollar, e1->name);
			cpy(e1->name, dollar);
			$$ = (char *)e1;
		  }
		| NOT Exp{
			Item *e1 = (Item *)$2;
			int t1 = e1->type;
			if (TYPE_INT == t1 || TYPE_VAR_INT == t1){
				e1->type = TYPE_INT;
			}
			else {
				printf("Error type 7 at Line %d: not match of %s\n", ((Leaf *)$2)->line, ((Leaf *)$2)->token);
				e1->type = TYPE_INT;
			}

			// middle start
			printExp(&e1);
			char *dollar = getTempVar();
			printf("%s := !%s\n", dollar, e1->name);
			cpy(e1->name, dollar);
			$$ = (char *)e1;
		  }
		| ID LP {
			Item *fun = getItem(((Leaf *)$1)->val.val_name);
			if (fun == NULL) {
				printf("Error type 2 at Line %d: function %s undefined\n", ((Leaf *)$1)->line, ((Leaf *)$1)->val.val_name);
			}
			else {
				if (fun->type != TYPE_FUNCTION)
					printf("Error type 11 at Line %d: %s not a function\n", ((Leaf *)$1)->line, ((Leaf *)$1)->val.val_name);
			}
			fun = NULL;
	 	  }
		  Args RP {
			  Item *def_args = getArgs(((Leaf *)$1)->val.val_name);
			  Item *in_args = (Item *)$4;

			  if (!cmpArgs(def_args, in_args)){
				  printf("Error type 9 at Line %d: args not match of function %s\n", ((Leaf *)$1)->line, ((Leaf *)$1)->val.val_name);
			  }
			  // middle start
			  Item *trace = in_args;
			  while (trace != NULL){
				  printExp(&trace);
				  printf("ARG %s\n", trace->name);
				  trace = trace->next;
			  }

			  Item *fun = getItem(((Leaf *)$1)->val.val_name);
			  Item *dollar = newItem();
			  if (fun != NULL){
				  dollar->type = fun->ret_type;
				  cpy(dollar->type_name, fun->ret_type_name);
				  memset(dollar->name, 0, ID_MAX_LEN);
				  sprintf(dollar->name, "CALL %s", ((Leaf *)$1)->val.val_name);
			  }
			  $$ = (char *)dollar;
		  }
		| Exp LB Exp RB	{
			Item *var = (Item *)$1;
			Item *var3 = (Item *)$3;
			Item *def = getItem(var->name);
			if (var->type == TYPE_VAR_INT || var->type == TYPE_VAR_FLOAT || var->type == TYPE_VAR_STRUCT){
				if (var3->type == TYPE_INT || var3->type == TYPE_VAR_INT){
					var->dimension ++;
					if (var->dimension > def->dimension){
						printf("Error type 10 at Line %d: %s not array or dimension not match\n", ((Leaf *)$2)->line, var->name);
					}
					else {
						// middle start
						int dim_step = 1;
						int i;
						for (i = var->dimension; i < def->dimension; i ++) dim_step *= def->dim_max[i];
						dim_step *= getTypeSize(def);
						char *tvar0 = getTempVar();
						if (var->dimension == 1) {
							if (var3->dimension > 0) {
								char *tvar00 = getTempVar();
								printf("%s := &%s\n", tvar00, var3->name);
								printf("%s := %s + %s\n", tvar00, tvar00, var3->offset);
								printf("%s := *%s\n", tvar00, tvar00);
								memset(var3->name, 0, ID_MAX_LEN);
								sprintf(var3->name, "%s", tvar00);
								var3->dimension = 0;
							}
							printf("%s := %s * #%d\n", tvar0, var3->name, dim_step);
							sprintf(var->offset, "%s", tvar0);
						}
						else {
							if (var3->dimension > 0) {
								char *tvar00 = getTempVar();
								printf("%s := &%s\n", tvar00, var3->name);
								printf("%s := %s + %s\n", tvar00, tvar00, var3->offset);
								printf("%s := *%s\n", tvar00, tvar00);
								memset(var3->name, 0, ID_MAX_LEN);
								sprintf(var3->name, "%s", tvar00);
								var3->dimension = 0;
							}
							printf("%s := %s * #%d\n", tvar0, var3->name, dim_step);
							char *tvar1 = getTempVar();
							printf("%s := %s + %s\n", tvar1, tvar0, var->offset);
							memset(var->offset, 0, 2*ID_MAX_LEN);
							sprintf(var->offset, "%s", tvar1);
						}
					}
				}
				else {
					printf("Error type 12 at Line %d: error type between [ ]\n", ((Leaf *)$2)->line);
				}
			}
			else printf("Error type 10 at Line %d: %s not array\n", ((Leaf *)$2)->line, var->name);
			$$ = (char *)var;
		  }
		| Exp DOT ID {
			Leaf *mem = (Leaf *)$3;
			Item *exp = (Item *)$1;
			if (exp->type != TYPE_VAR_STRUCT) {
				printf("Error type 13 at Line %d: %s not a struct\n", ((Leaf *)$2)->line, exp->name);
			}
			else {
				Item *component = getItem(mem->val.val_name);
				Item *def = getItem(exp->type_name);
				if (component == NULL || component->scope != def) {
					printf("Error type 14 at Line %d: %s not %s 's component\n", ((Leaf *)$2)->line, component->name, exp->type_name);
				}
				else {
					Item *dollar = newItem();
					memcpy(dollar, component, sizeof(Item));
					dollar->dimension = 0;
					dollar->dim_max = NULL;

					// middle start
					if (NULL != def){
						Item *mems = getStructMember((char *)def);
						Item *trace = mems;
						int offset = 0;
						while (trace != NULL && NULL != mem && 0 != cmp(trace->name, mem->val.val_name)) {
							int arr_num = getArrayNum(trace);
							if (arr_num > 0) offset += arr_num*getTypeSize(trace);
							else offset += getTypeSize(trace);
							trace = trace->next;
						}
						assert(trace != NULL);

						char *tvar = getTempVar();
						assert(NULL != exp);

						// array cond
						if (exp->dimension > 0) {
							char *tvar00 = getTempVar();
							printf("%s := &%s\n", tvar00, exp->name);
							printf("%s := %s + %s\n", tvar00, tvar00, exp->offset);
							printf("%s := *%s\n", tvar00, tvar00);
							memset(exp->name, 0, ID_MAX_LEN);
							sprintf(exp->name, "%s", tvar00);
							exp->dimension = 0;
						}

						printf("%s := &%s\n", tvar, exp->name);
						char *tvar1 = getTempVar();
						printf("%s := %s + #%d\n", tvar1, tvar, offset);
						char *tvar2 = getTempVar();
						printf("%s := *%s\n", tvar2, tvar1);
						cpy(dollar->name, tvar2);
					}

					$$ = (char *)dollar;
				}
			}
		  }
		| ID { 
			if (!isContain(((Leaf *)$1)->val.val_name)) {
				printf("Error type 1 at Line %d: var %s undefined\n", ((Leaf *)$1)->line, ((Leaf *)$1)->val.val_name);
				$$ = (char *)newItem();
			}
			else {
				// 从符号表里取数
				Item *var = getItem(((Leaf *)$1)->val.val_name);
				Item *dd = newItem();

				if (var != NULL){
					memcpy(dd, var, sizeof(Item));
					dd->dimension = 0;
					dd->dim_max = NULL;
				}
				$$ = (char *)dd;
			}
		  }
		| INT {
			Item *tmp = newItem();
			tmp->line = ((Leaf *)$1)->line;
			tmp->type = TYPE_INT;
			sprintf(tmp->name, "#%d", ((Leaf *)$1)->val.val_int);
			$$ = (char *)tmp;
//			printf("exp type=%d\n", ((Item *)$$)->type);
		  }
		| FLOAT {
			Item *tmp = newItem();
			tmp->line = ((Leaf *)$1)->line;
			tmp->type = TYPE_FLOAT;
			sprintf(tmp->name, "#%lf", ((Leaf *)$1)->val.val_double);
			$$ = (char *)tmp;
//			printf("exp type=%d\n", ((Item *)$$)->type);
		  }
		| error RP %prec RP_ERR		{}
		| Exp error			{}
		;

Args		: Exp COMMA Args {
			  Item *e1 = (Item *)$1;
			  Item *e3 = (Item *)$3;
			  e1->next = e3;
			  $$ = (char *)e1;
			 // printf("Args->Exp COMMA Args: %lx->%lx\n", d1, d3);
			 // printf("%lx:%d Item:\targs_num=%d scope=%lx type=%d type_name=%s ret_type=%d \n\tret_type_name=%s name=%s dimension=%d line=%d\n", (unsigned long)d1, d1->line, d1->args_num, (unsigned long)d1->scope, d1->type, d1->type_name, d1->ret_type, d1->ret_type_name, d1->name, d1->dimension, d1->line);
		  }
		| Exp {
			Item *e = (Item *)$1;
			$$ = $1;
			((Item *)$$)->next = NULL;
		//	printf("Args->Exp: %lx->%lx\n", $$, NULL);
			Item *d1 = (Item *)$$;
		//	  printf("%lx:%d Item:\targs_num=%d scope=%lx type=%d type_name=%s ret_type=%d \n\tret_type_name=%s name=%s dimension=%d line=%d\n", (unsigned long)d1, d1->line, d1->args_num, (unsigned long)d1->scope, d1->type, d1->type_name, d1->ret_type, d1->ret_type_name, d1->name, d1->dimension, d1->line);
		  }
		| /**/ {$$ = NULL;}
		;

%%
yyerror(char *msg){
	fprintf(stderr, "Error type B at Line %d: %s\n", yylineno, msg);
}

