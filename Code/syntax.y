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
			  // 这里的循环讲连续定义的全局变量插入符号表 
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
		  }
		| VarDec COMMA ExtDecList	{
			$$ = $1;
			((Item *)$$)->next = (Item *)$3;
		  }
		;

Specifier	: TYPE {  // 基本类型的变量
			  $$ = (char *)newItem();
			  if (cmp(((Leaf *)$1)->val.val_name, "int") == 0) ((Item *)$$)->type = TYPE_VAR_INT;
			  else if (cmp(((Leaf *)$1)->val.val_name, "float") == 0) ((Item *)$$)->type = TYPE_VAR_FLOAT;
			  ((Item *)$$)->scope = getScope();
			  // 在做符号表插入操作之前，先暂时保存变量的类型
			  saveTempType((Item *)$$);
		  }
		| StructSpecifier {
			// 结构体类型的变量
			$$ = $1;
			((Item *)$$)->type = TYPE_VAR_STRUCT;
			// 暂时保存变量的类型
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
			// 给匿名结构体一个名字，让结构体等价判断更方便【名等价一定结构等价，反过来不是
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
			  // 变量名
			  $$ = (char *)newItem();
			  ((Item *)$$)->scope = getScope();
			  cpy(((Item *)$$)->name, ((Leaf *)$1)->val.val_name);
			  ((Item *)$$)->line = ((Leaf *)$1)->line;
			  ((Item *)$$)->dimension = 0;
		  }
		| VarDec LB INT {
			// 数组变量
			$$ = $1;
			((Item *)$$)->dimension ++;		//保存数组变量的维数，例如a[3][5]最后保存的维数=2
			int *tmp = ((Item *)$$)->dim_max;	//保存数组变量每一维的最大标号，使用动态数组来保存，因为无法确定维数
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
			  ((Item *)$2)->type = TYPE_FUNCTION;	//所有的变量都保存在一张符号表里，设置类型以区分符号类型
			  ((Item *)$2)->line = ((Leaf *)$1)->line;
			  if ($1 != NULL) cpy(((Item *)$2)->name, ((Leaf *)$1)->val.val_name);
			  else printf("$1 is null\n");
			  insertTable((Item *)$2);
			  setScope((Item *)$2);

			  // middle start，函数定义的中间代码生成
			  Midcode *code = newMidcode();
			  sprintf(code->sentence, "FUNCTION %s :\n", ((Leaf *)$1)->val.val_name);
		  } VarList RP{
			  $$ = $2;
			  setArgsNumber($2);
			  ((Item *)$$)->line = ((Leaf *)$1)->line;
			  setScope(NULL);
			  Item *paras = (Item *)$4;
			  while (paras != NULL){
				  // 因为设置了结构体使用引用传递，所以实际上传的是地址，所以需要在传完参数之后马上设置*来取值
				  // 直接修改变量名其实不太好，不过这样最方便
				  if (paras->type == TYPE_VAR_STRUCT){
					  int length = strlen(paras->name);
					  int i;
					  for (i = length; i > 0; i --) paras->name[i] = paras->name[i-1];
					  paras->name[0] = '*';
				  }
				  paras = paras->next;
			  }
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

			// middle start，函数定义中间代码生成
			Midcode *code = newMidcode();
			sprintf(code->sentence, "FUNCTION %s :\n", ((Leaf *)$1)->val.val_name);
		  }
		| error RP %prec FUN_ERR	{}
		;

VarList		: ParamDec COMMA VarList {
			  // 参数列表定义
			  ((Item *)$1)->next = (Item *)$3;
			  $$ = $1;
			  // middle start
			  Item *para = (Item *)$1;
			  Item *tp = getTempType();
			  int size = getTypeSize(tp);
			  Midcode *code = newMidcode();
			  sprintf(code->sentence, "PARAM %s\n", para->name);
			  /* 使用值传递会获得一个超长的参数列表，所以最后还是放弃了
			  if (size > 4){
				  char *addr = getTempVar();
				  Midcode *code = newMidcode();
				  sprintf(code->sentence, "%s := &%s\n", addr, para->name);
				  while (size > 4){
					  code = newMidcode();
					  sprintf(code->sentence, "%s := %s + #4\n", addr, addr);
					  sprintf(code->sentence, "PARAM *%s\n", addr);
				  }
			  }*/
		  }
		| ParamDec {
			$$ = $1;
			// middle start
			Item *para = (Item *)$1;
			Item *tp = getTempType();
			int size = getTypeSize(tp);

			Midcode *code = newMidcode();
			sprintf(code->sentence, "PARAM %s\n", para->name);
			/* 使用值传递会获得一个超长的参数列表，所以最后还是放弃了
				if (size > 4){
				char *addr = getTempVar();
				Midcode *code = newMidcode();
				sprintf(code->sentence, "%s := &%s\n", addr, para->name);
				while (size > 4){
					code = newMidcode();
					sprintf(code->sentence, "%s := %s + #4\n", addr, addr);
					sprintf(code->sentence, "PARAM *%s\n", addr);
				}
			}*/
		 }
		;

ParamDec	: Specifier VarDec {
			  // middle start
			  Item *tp = (Item *)$1;
			  saveTempType(tp);

			  Item *trace = (Item *)$2;
			  Item *item = trace;
			  if (trace != NULL){
				  item->type = ((Item *)$1)->type;
				  cpy(item->type_name, ((Item *)$1)->type_name);
				  insertTable(item);

				  Item *dollar = newItem();
				  memcpy(dollar, $2, sizeof(Item));
				  dollar->next = NULL;
				  dollar->dim_max = NULL;
				  $$ = (char *)dollar;
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
			  }
		  }
		| error RC %prec COM_ERR	{}
		;

StmtList	: Stmt {
			  // middle goto
			  char *label = newTagName();
			  setM(label);
			  Item *st = (Item *)$1;
			  backpatchList(st->nextlist, getM());	//回填nextlist
		  } StmtList {
			  $$ = $3;
		  }
		| /* empty */ { 
			// 后来发现这里会与IF ELSE语句的回填专用符号N产生冲突，因为两个都可以产生空语句
			$$ = (char *)newItem(); 
		  }
		;

Stmt		: Exp SEMI {
			  $$ = $1;

			  // middle start
			  Item *e1 = (Item *)$1;
			  // 不能直接调用函数，而是一定要有个返回值。。。这什么诡异设定
			  if (memcmp(e1->name, "CALL", 4) == 0){
				  printExp(&e1);
				  char *tvar = getTempVar();
				  Midcode *code = newMidcode();
				  sprintf(code->sentence, "%s := %s\n", tvar, e1->name);
			  }
		  }
		| CompSt {$$ = $1;}
		| RETURN Exp SEMI {
			$$ = $2;
			Item *t0 = (Item *)$$;
			Item *t2 = (Item *)$2;
			t0->result_type = t2->type;
			cpy(t0->result_type_name, t2->type_name);

			// middle start
			// 函数成功返回
			printExp(&t2);
			Midcode *code = newMidcode();
			sprintf(code->sentence, "RETURN %s\n", t2->name);
		  }
		| IF LP Exp RP M Stmt %prec LOWER_THAN_ELSE {
			Item *cond = (Item *)$3;

			// middle goto
			if (cond->truelist == NULL && cond->falselist == NULL){
				Midcode *code = newMidcode();
				sprintf(code->sentence, "IF %s != #0 GOTO @\n", cond->name);
				cond->truelist = mergeNode(cond->truelist, code);
				code = newMidcode();
				sprintf(code->sentence, "GOTO @\n");
				cond->falselist = mergeNode(cond->falselist, code);
			}
			Item *m = (Item *)$5;
			backpatchList(cond->truelist, m->name);

			// middle goto
			Item *dollar = newItem();
			Item *st = (Item *)$6;
			dollar->nextlist = mergeList(dollar->nextlist, cond->falselist);
			dollar->nextlist = mergeList(dollar->nextlist, st->nextlist);

			$$ = (char *)dollar;
		  }
		| IF LP Exp RP M Stmt ELSE {
			// 其实这一段代码是N产生式的工作，但是幸好ELSE其实是个无意义的符号，所以放在这里恰好可以消除冲突
			Midcode *code = newMidcode();
			sprintf(code->sentence, "GOTO @\n");
			Item *dollar = newItem();
			dollar->nextlist = mergeNode(dollar->nextlist, code);
			$7 = (char *)dollar;
		  } M Stmt{
			// middle goto
			Item *cond = (Item *)$3;
			if (cond->truelist == NULL && cond->falselist == NULL){
				Midcode *code = newMidcode();
				sprintf(code->sentence, "IF %s != #0 GOTO @\n", cond->name);
				cond->truelist = mergeNode(cond->truelist, code);
				code = newMidcode();
				sprintf(code->sentence, "GOTO @\n");
				cond->falselist = mergeNode(cond->falselist, code);
			}

			Item *m1 = (Item *)$5;
			Item *m2 = (Item *)$9;
			backpatchList(cond->truelist, m1->name);
			backpatchList(cond->falselist, m2->name);

			// middle goto
			Item *s1 = (Item *)$6;
			Item *s2 = (Item *)$10;
			Item *n = (Item *)$7;

			Item *dollar = newItem();
			s1->nextlist = mergeList(s1->nextlist, n->nextlist);
			dollar->nextlist = mergeList(dollar->nextlist, s1->nextlist);
			dollar->nextlist = mergeList(dollar->nextlist, s2->nextlist);

			$$ = (char *)dollar;
		  }
		| WHILE LP {
			// middle goto
			char *label = newTagName();
			Midcode *m1 = newMidcode();
			sprintf(m1->sentence, "LABEL %s :\n", label);
			$2 = label;
		  } Exp RP{
			Item *cond = (Item *)$4;
			//middle start
			// middle goto
			printExp(&cond);
			if (getBoolean() == 0){
				Midcode *code = newMidcode();
				sprintf(code->sentence, "IF %s != #0 GOTO @\n", cond->name);
				cond->truelist = mergeNode(cond->truelist, code);
				code = newMidcode();
				sprintf(code->sentence, "GOTO @\n");
				cond->falselist = mergeNode(cond->falselist, code);
			}

			if (cond->type != TYPE_INT && cond->type != TYPE_VAR_INT)
				printf("Error type ? at Line %d: error condition type\n", ((Leaf *)$1)->line);
			Item *tmp = newItem();
			tmp->line = ((Leaf *)$1)->line;
			tmp->type = TYPE_WHILE;
			insertTable(tmp);
			setScope(tmp);

			// middle goto
			char *label = newTagName();
			setM(label);
			backpatchList(cond->truelist, getM());

		  }Stmt{
			if (getScope() != NULL) setScope(getScope()->scope);
			else setScope(NULL);

			// middle goto
			Midcode *loop = newMidcode();
			sprintf(loop->sentence, "GOTO %s\n", $2);
			Item *st = (Item *)$7;
			backpatchList(st->nextlist, $2);

			Item *cond = (Item *)$4;
			Item *dollar = newItem();
			dollar->nextlist = cond->falselist;
			$$ = (char *)dollar;
		  }
		| error SEMI %prec STM_ERR	{}
		;

M		: /**/ {
			       char *label = newTagName();
			       Midcode *code = newMidcode();
			       sprintf(code->sentence, "LABEL %s :\n", label);
			       Item *dollar = newItem();
			       dollar->nextlist = mergeNode(dollar->nextlist, code);
			       sprintf(dollar->name, "%s", label);
			       $$ = (char *)dollar;
		       }
		;

DefList		: Def DefList			{}
		| /* empty */ 			{}
		;

Def		: Specifier DecList SEMI {
			  Item *t1 = (Item *)$1;

			  Item *trace = (Item *)$2;
			  Item *item = trace;
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
				  if (arr_num > 0) {
					  Midcode *code = newMidcode();
					  sprintf(code->sentence, "DEC %s %d\n", var->name, arr_num*getTypeSize(tp));
				  }
				  else if (NULL != tp && TYPE_VAR_STRUCT == tp->type) {
					  Midcode *code = newMidcode();
					  sprintf(code->sentence, "DEC %s %d\n", var->name, getTypeSize(tp));
				  }
			  }
		  }
		| VarDec {
			// middle start
			if (getScope() != NULL && (getScope()->type == TYPE_STRUCT || getScope()->type == TYPE_VAR_STRUCT));
			else{
				Item *tp = getTempType();
				Item *var = (Item *)$1;
				int arr_num = getArrayNum(var);
				if (arr_num > 0) {
					Midcode *code = newMidcode();
					sprintf(code->sentence, "DEC %s %d\n", var->name, arr_num*getTypeSize(tp));
				}
				else if (NULL != tp && TYPE_VAR_STRUCT == tp->type) {
					Midcode *code = newMidcode();
					sprintf(code->sentence, "DEC %s %d\n", var->name, getTypeSize(tp));
				}
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
				  if (memcmp(e2->name, "CALL", 4) == 0) {
					  char *tvar1 = getTempVar();
					  Midcode *c = newMidcode();
					  sprintf(c->sentence, "%s := %s\n", tvar1, e2->name);
					  memset(e2->name, 0, ID_MAX_LEN);
					  sprintf(e2->name, "%s", tvar1);
				  }
				  Midcode *code = newMidcode();
				  sprintf(code->sentence, "%s := %s\n", e1->name, e2->name);
			  }
			  else if (TYPE_VAR_STRUCT == tp->type){
				  printExp(&e2);
				  int size = getTypeSize(tp);
				  int i = 0;
				  char *tvar1 = getTempVar();
				  char *tvar2 = getTempVar();
				  Midcode *code = newMidcode();
				  if (e1->name[0] == '*') sprintf("%s := %s\n", tvar1, (e1->name)+1);
				  else sprintf(code->sentence, "%s := &%s\n", tvar1, e1->name);
				  code = newMidcode();
				  if (e1->name[0] == '*') sprintf("%s := %s\n", tvar1, (e1->name)+1);
				  else sprintf(code->sentence, "%s := &%s\n", tvar2, e2->name);
				  for (; i < size; i += 4){
					  code = newMidcode();
					  sprintf(code->sentence, "*%s := *%s\n", tvar1, tvar2);
					  if (i+4 < size){
						  code = newMidcode();
						  sprintf(code->sentence, "%s := %s + #4\n", tvar1, tvar1);
						  code = newMidcode();
						  sprintf(code->sentence, "%s := %s + #4\n", tvar2, tvar2);
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
					  if (memcmp(e2->name, "CALL", 4) == 0) {
						  char *tvar1 = getTempVar();
						  Midcode *c = newMidcode();
						  sprintf(c->sentence, "%s := %s\n", tvar1, e2->name);
						  memset(e2->name, 0, ID_MAX_LEN);
						  sprintf(e2->name, "%s", tvar1);
					  }
					  Midcode *code = newMidcode();
					  sprintf(code->sentence, "%s := %s\n", e1->name, e2->name);
				  }
				  else if (TYPE_VAR_FLOAT == t1 && (TYPE_VAR_FLOAT == t2 || TYPE_FLOAT == t2)){
					  // middle start
					  printExp(&e1);
					  printExp(&e2);
					  if (memcmp(e2->name, "CALL", 4) == 0) {
						  char *tvar1 = getTempVar();
						  Midcode *c = newMidcode();
						  sprintf(c->sentence, "%s := %s\n", tvar1, e2->name);
						  memset(e2->name, 0, ID_MAX_LEN);
						  sprintf(e2->name, "%s", tvar1);
					  }
					  Midcode *code = newMidcode();
					  sprintf(code->sentence, "%s := %s\n", e1->name, e2->name);
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
						  Midcode *code = newMidcode();
						  if (e1->name[0] == '*') sprintf("%s := %s\n", tvar1, (e1->name)+1);
						  else sprintf(code->sentence, "%s := &%s\n", tvar1, e1->name);
						  code = newMidcode();
						  if (e1->name[0] == '*') sprintf("%s := %s\n", tvar1, (e1->name)+1);
						  else sprintf(code->sentence, "%s := &%s\n", tvar2, e2->name);
						  for (; i < size; i += 4){
							  code = newMidcode();
							  sprintf(code->sentence, "*%s := *%s\n", tvar1, tvar2);
							  if (i+4 < size){
								  code = newMidcode();
								  sprintf(code->sentence, "%s := %s + #4\n", tvar1, tvar1);
								  code = newMidcode();
								  sprintf(code->sentence, "%s := %s + #4\n", tvar2, tvar2);
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
			  // 不是bool表达式
			  setBoolean(0);
		  }
		| Exp AND {
			// middle goto
			char *label = newTagName();
			setM(label);
		  } Exp {
			Item *e1 = (Item *)$1;
			Item *e2 = (Item *)$4;
			int t1 = e1->type;
			int t2 = e2->type;
			if ((TYPE_INT == t1 || TYPE_VAR_INT == t1) && (TYPE_INT == t2 || TYPE_VAR_INT == t2)){
				e1->type = TYPE_INT;
			}
			else {
				printf("Error type 7 at Line %d: not match of %s\n", ((Leaf *)$2)->line, ((Leaf *)$2)->token);
				e1->type = TYPE_INT;
			}

			// middle goto
			backpatchList(e1->truelist, getM());

			//middle start
			printExp(&e1);
			printExp(&e2);
			char *dollar = getTempVar();
			Midcode *code = newMidcode();
			sprintf(code->sentence, "%s := %s && %s\n", dollar, e1->name, e2->name);
			cpy(e1->name, dollar);

			// middle goto
			e1->truelist = e2->truelist;
			e1->falselist = mergeList(e1->falselist, e2->falselist);
			$$ = (char *)e1;

			// 不是bool表达式
			setBoolean(0);
		  }
		| Exp OR {
			char *label = newTagName();
			setM(label);
		  } Exp {
			Item *e1 = (Item *)$1;
			Item *e2 = (Item *)$4;
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
			Midcode *code = newMidcode();
			sprintf(code->sentence, "%s := %s || %s\n", dollar, e1->name, e2->name);
			cpy(e1->name, dollar);

			// middle goto
			backpatchList(e1->falselist, getM());
			e1->truelist = mergeList(e1->truelist, e2->truelist);
			e1->falselist = e2->falselist;

			$$ = (char *)e1;
			// 不是bool表达式
			setBoolean(0);
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
			// middle goto
			printExp(&e1);
			printExp(&e2);
			Midcode *code = newMidcode();
			sprintf(code->sentence, "IF %s %s %s GOTO @\n", e1->name, ((Leaf *)$2)->val.val_name, e2->name);
			e1->truelist = mergeNode(e1->truelist, code);
			code = newMidcode();
			sprintf(code->sentence, "GOTO @\n");
			e1->falselist = mergeNode(e1->falselist, code);
			$$ = (char *)e1;
			// 是bool表达式
			setBoolean(1);
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
			Midcode *code = newMidcode();
			sprintf(code->sentence, "%s := %s + %s\n", dollar, e1->name, e2->name);
			cpy(e1->name, dollar);
			$$ = (char *)e1;
			// 不是bool表达式
			setBoolean(0);
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
			Midcode *code = newMidcode();
			sprintf(code->sentence, "%s := %s - %s\n", dollar, e1->name, e2->name);
			cpy(e1->name, dollar);
			$$ = (char *)e1;
			// 不是bool表达式
			setBoolean(0);
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
			Midcode *code = newMidcode();
			sprintf(code->sentence, "%s := %s * %s\n", dollar, e1->name, e2->name);
			cpy(e1->name, dollar);
			$$ = (char *)e1;
			// 不是bool表达式
			setBoolean(0);
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
			Midcode *code = newMidcode();
			sprintf(code->sentence, "%s := %s * %s\n", dollar, e1->name, e2->name);
			cpy(e1->name, dollar);
			$$ = (char *)e1;
			// 不是bool表达式
			setBoolean(0);
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
			Midcode *code = newMidcode();
			sprintf(code->sentence, "%s := #0 - %s\n", dollar, e1->name);
			cpy(e1->name, dollar);
			$$ = (char *)e1;
			// 不是bool表达式
			setBoolean(0);
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
			// middle goto
			printExp(&e1);
			codeItem *tl = e1->truelist;
			codeItem *fl = e1->falselist;
			e1->truelist = fl;
			e1->falselist = tl;

			char *dollar = getTempVar();
			Midcode *code = newMidcode();
			sprintf(code->sentence, "%s := !%s\n", dollar, e1->name);
			cpy(e1->name, dollar);
			$$ = (char *)e1;
			// 不是bool表达式
			setBoolean(0);
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
			  Item *fun = getItem(((Leaf *)$1)->val.val_name);
			  Item *dollar = newItem();
			  if (fun != NULL){
				  dollar->type = fun->ret_type;
				  cpy(dollar->type_name, fun->ret_type_name);
			  }

			  if (cmp(((Leaf *)$1)->val.val_name, "read") == 0) {
				  char *tvar0 = getTempVar();
				  Midcode *code = newMidcode();
				  sprintf(code->sentence, "READ %s\n", tvar0);
				  memset(dollar->name, 0, ID_MAX_LEN);
				  sprintf(dollar->name, "%s", tvar0);
			  }
			  else {
				  Item *trace = in_args;
				  while (trace != NULL){
/*					  if (trace->type == TYPE_VAR_STRUCT){
						  int size = getTypeSize(trace);
						  if (size > 4){
							  char *addr = getTempVar();
							  Midcode *code = newMidcode();
							  sprintf(code->sentence, "%s := &%s\n", addr, trace->name);
							  size -= 4;
							  while (size >= 4){
								  char *ttvar = getTempVar();
								  code = newMidcode();
								  sprintf(code->sentence, "%s := %s + #%d\n", ttvar, addr, size);
								  code = newMidcode();
								  sprintf(code->sentence, "ARG *%s\n", ttvar);
								  size -= 4;
							  }
						  }
					  }*/
					  printExp(&trace);
					  Item *def = getItem(trace->name);
					  if (def != NULL && def->type == TYPE_VAR_STRUCT && def->scope != NULL && def->scope->type == TYPE_FUNCTION){
						  Item *as = getArgs(def->scope->name);
						  if (isParameter(trace, as)) {
							  int length = strlen(trace->name);
							  int i;
							  for (i = length; i > 0; i --) trace->name[i] = trace->name[i-1];
							  trace->name[0] = '*';
						  }
					  }

					  Midcode *code = newMidcode();
					  if (trace->type == TYPE_VAR_STRUCT) {
						  if (trace->name[0] == '*') sprintf(code->sentence, "ARG %s\n", trace->name+1);
						  else sprintf(code->sentence, "ARG &%s\n", trace->name);
					  }
					  else sprintf(code->sentence, "ARG %s\n", trace->name);
					  trace = trace->next;
				  }
				  if (fun != NULL){
					  if (cmp(fun->name, "write") == 0){
						  dollar->type = TYPE_INT;
						  Midcode *code = newMidcode();
						  sprintf(code->sentence, "WRITE %s\n", in_args->name);
						  memset(dollar->name, 0, ID_MAX_LEN);
						  sprintf(dollar->name, "#0");
					  }
					  else {
						  dollar->type = fun->ret_type;
						  cpy(dollar->type_name, fun->ret_type_name);
						  memset(dollar->name, 0, ID_MAX_LEN);
						  sprintf(dollar->name, "CALL %s", ((Leaf *)$1)->val.val_name);
					  }
				  }
			  }

			  $$ = (char *)dollar;
			// 不是bool表达式
			setBoolean(0);
		  }
		| Exp LB Exp RB	{
			Item *var = (Item *)$1;
			Item *var3 = (Item *)$3;
			Item *def = getItem(var->name);
			if (def == NULL) def = getItem(var->def_name);
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
								Midcode *code = newMidcode();
								if (var3->name[0] == '*') 
									sprintf(code->sentence, "%s := %s\n", tvar00, (var3->name)+1);
								else sprintf(code->sentence, "%s := &%s\n", tvar00, var3->name);
								code = newMidcode();
								sprintf(code->sentence, "%s := %s + %s\n", tvar00, tvar00, var3->offset);
								code = newMidcode();
								sprintf(code->sentence, "%s := *%s\n", tvar00, tvar00);
								memset(var3->name, 0, ID_MAX_LEN);
								sprintf(var3->name, "%s", tvar00);
								var3->dimension = 0;
							}
							Midcode *code = newMidcode();
							sprintf(code->sentence, "%s := %s * #%d\n", tvar0, var3->name, dim_step);
							sprintf(var->offset, "%s", tvar0);
						}
						else {
							printExp(&var3);
							Midcode *code = newMidcode();
							sprintf(code->sentence, "%s := %s * #%d\n", tvar0, var3->name, dim_step);
							char *tvar1 = getTempVar();
							code = newMidcode();
							sprintf(code->sentence, "%s := %s + %s\n", tvar1, tvar0, var->offset);
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
			// 不是bool表达式
			setBoolean(0);
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
					if (exp->scope != NULL && exp->scope->type == TYPE_FUNCTION){
						Item *as = getArgs(exp->scope->name);
						if (isParameter(exp, as)){
							int length = strlen(exp->name);
							int i;
							for(i = length; i > 0; i --) exp->name[i] = exp->name[i-1];
							exp->name[0] = '*';
						}
					}

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
						printExp(&exp);
						Midcode *code = newMidcode();
						if (exp->name[0] == '*') sprintf(code->sentence, "%s := %s\n", tvar, exp->name+1);
						else sprintf(code->sentence, "%s := &%s\n", tvar, exp->name);
						char *tvar1 = getTempVar();
						code = newMidcode();
						sprintf(code->sentence, "%s := %s + #%d\n", tvar1, tvar, offset);
						memset(dollar->def_name, 0, ID_MAX_LEN);
						cpy(dollar->def_name, mem->val.val_name);
						memset(dollar->name, 0, ID_MAX_LEN);
						sprintf(dollar->name, "*%s", tvar1);
					}

					$$ = (char *)dollar;
				}
			}
			// 不是bool表达式
			setBoolean(0);
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
			// 不是bool表达式
			setBoolean(0);
		  }
		| INT {
			Item *tmp = newItem();
			tmp->line = ((Leaf *)$1)->line;
			tmp->type = TYPE_INT;
			sprintf(tmp->name, "#%d", ((Leaf *)$1)->val.val_int);
			$$ = (char *)tmp;
			// 不是bool表达式
			setBoolean(0);
		  }
		| FLOAT {
			Item *tmp = newItem();
			tmp->line = ((Leaf *)$1)->line;
			tmp->type = TYPE_FLOAT;
			sprintf(tmp->name, "#%lf", ((Leaf *)$1)->val.val_double);
			$$ = (char *)tmp;
			// 不是bool表达式
			setBoolean(0);
		  }
		| error RP %prec RP_ERR		{}
		| Exp error			{}
		;

Args		: Exp COMMA Args {
			  Item *e1 = (Item *)$1;
			  Item *e3 = (Item *)$3;
			  e1->next = e3;
			  $$ = (char *)e1;
		  }
		| Exp {
			Item *e = (Item *)$1;
			$$ = $1;
			((Item *)$$)->next = NULL;
		  }
		| /**/ {$$ = NULL;}
		;

%%
yyerror(char *msg){
	fprintf(stderr, "Error type B at Line %d: %s\n", yylineno, msg);
}

