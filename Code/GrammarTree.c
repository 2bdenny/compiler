#include "GrammarTree.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

Item *table = NULL;
Item *tail = NULL;
Item *scope = NULL;
int count = 0;

Tree *forest = NULL;
int getError = 0;

// 制造一个词
Leaf *makeLeaf(int line, int valno, int terminal, char *token, Value val){
	Leaf *x = (Leaf *)malloc(sizeof(Leaf));
	x->line = line;
	x->valno = valno;
	x->terminal = terminal;
	memset(x->token, 0, 20);
	memcpy(x->token, token, strlen(token));
	x->val = val;
	x->left = NULL;
	x->right = NULL;
	return x;
}

// 限制：对于一个表达式，规约的时候，必须从左到右地增加child
int addChild(Leaf *parent, Leaf *child){
	if (parent->left == NULL) {
		parent->left = child;
		return true;
	}
	else {
		Leaf *brother = parent->left;
		while (brother->right != NULL) brother = brother->right;
		brother->right = child;
		return true;
	}

	return false;
}

// 展示一棵树
void display(Leaf *tree, int ntab){
	if (getError == 1) return;
	if (tree != NULL) {
		int i = 0;
		for (; i < ntab; i ++) printf("  ");
		if (tree->terminal == 0) printf("%s (%d)\n", tree->token, tree->line);
		else{
			printf("%s", tree->token);
			switch(tree->valno){
				case -1: printf("\n"); break;
				case 0: printf(": %f\n", tree->val.val_double); break;
				case 1: printf(": %d\n", tree->val.val_int); break;
				case 2: printf(": %f\n", tree->val.val_float); break;
				case 3: printf(": %s\n", tree->val.val_name); break;
			}
		}
		display(tree->left, ntab+1);
		display(tree->right, ntab);
	}
}


// 在森林中增加一棵树
int addTree(Leaf *tree){
	if (forest == NULL) {
		forest = (Tree *)malloc(TREE_LEN);
		forest->tree = tree;
		forest->next = NULL;
		return true;
	}
	else {
		Tree *tmp = (Tree *)malloc(TREE_LEN);
		tmp->tree = tree;
		tmp->next = forest;
		forest = tmp;
		return true;
	}
	return false;
}
// 删除一棵树
int delTree(Leaf *tree){
	Tree *prev = NULL;
	Tree *tmp = forest;
	if (tmp == NULL) return false;
	else {
		while (tmp->tree != tree) {
			if (tmp->next == NULL) return false;
			else {
				prev = tmp;
				tmp = tmp->next;
			}
		}
		if (prev == NULL) {
			forest = forest->next;
			free(tmp);
			return true;
		}
		else {
			prev->next = tmp->next;
			free(tmp);
			return true;
		}
	}
	return false;
}
void displayTree(Tree *f){
	while (f != NULL){
		printf("%s\n", (f->tree)->token);
		f = f->next;
	}
}
void meetError(){
	getError = 1;
}
void destroy(Leaf **tree){
	Leaf *tmp = *tree;
	if (tmp == NULL) return;
	if (tmp->left != NULL) destroy(&(tmp->left));
	if (tmp->right != NULL) destroy(&(tmp->right));
	free(tmp);
	*tree = NULL;
}
void destroyForest(){
	while (forest != NULL){
		Tree *tmp = forest;
		forest = forest->next;
		destroy(&(tmp->tree));
		free(tmp);
	}
}
//-----------------------------------------------
//以上是lab1和lab2部分，下面开始是lab3部分
//-----------------------------------------------
//简化memcmp
int cmp(char *a, char *b){
	int maxLen = max(strlen(a), strlen(b));
	return memcmp(a, b, maxLen);
}
//简化memcpy
void *cpy(char *a, char *b){
	int maxLen = max(strlen(a), strlen(b));
	return memcpy(a, b, maxLen);
}
//获取符号表的符号的作用域
Item *getScope(){return scope;}
//设置作用域
void setScope(Item *new_scope){scope = new_scope;}
//获取一个新的符号表项
Item *newItem(){
	Item *result = (Item *)malloc(sizeof(Item));

	result->scope = NULL;
	result->number = count++;
	result->args_num = -1;

	result->type = -1;
	memset(result->type_name, 0, ID_MAX_LEN);
	result->ret_type = -1;
	memset(result->ret_type_name, 0, ID_MAX_LEN);
	memset(result->name, 0, ID_MAX_LEN);
	result->line = -1;

	result->result_type = -1;
	memset(result->result_type_name, 0, ID_MAX_LEN);

	result->dimension = -1;
	result->dim_max = NULL;
	memset(result->offset, 0, ID_MAX_LEN*2);
	result->next = NULL;
	memset(result->def_name, 0, ID_MAX_LEN);

	result->truelist = NULL;
	result->falselist = NULL;
	result->nextlist = NULL;

	return result;
}
//插入到符号表
void insertTable(Item *x){
	if (x != NULL){
		if (x->type == TYPE_STRUCT && isContain(x->type_name)) {
			printf("Error type 16 at Line %d: struct %s name has been used\n", x->line, x->type_name);
			return;
		}
		else if (x->type == TYPE_FUNCTION && isContain(x->name)){
			printf("Error type 4 at Line %d: function %s name has been used\n", x->line, x->name);
			return;
		}
		else if (x->type != TYPE_STRUCT  && x->type != TYPE_IF && x->type != TYPE_WHILE && x->type != TYPE_ELSE && isContain(x->name)){
			printf("Error type 3 at Line %d: var %s name has been used\n", x->line, x->name);
			return;
		}
		x->next = NULL;
		if (table == NULL) {
			table = x;
			tail = x;
		}
		else {
			tail->next = x;
			tail = x;
		}
	}
//	displayTable(table);
}
//展示符号表
void displayTable(Item *table){
	printf("--------Current Table---------\n");
	int number = -1;
	Item *trace = table;
	while (trace != NULL){
		printf("%lx:%d Item:\targs_num=%d scope=%lx type=%d type_name=%s ret_type=%d ret_type_name=%s \n\tname=%s dimension=%d line=%d next=%lx\n", (unsigned long)trace, trace->line, trace->args_num, (unsigned long)trace->scope, trace->type, trace->type_name, trace->ret_type, trace->ret_type_name, trace->name, trace->dimension, trace->line, (unsigned long)trace->next);
		printf("\n");
		trace = trace->next;
	}
	printf("------------------------------\n");
}
//符号表中是否已经存在这个符号
bool isContain(char *var){
	// 保证匿名结构体都可以被保存
	if (var == NULL || strlen(var) == 0) return false;
	Item *trace = table;
	while (trace != NULL){
		if (trace->type == TYPE_STRUCT && cmp(trace->type_name, var) == 0) return true;
		if (trace->type != TYPE_STRUCT && cmp(trace->name, var) == 0) return true;
		trace = trace->next;
	}
	return false;
}
//it是不是函数的参数，主要用在函数嵌套调用的时候
bool isParameter(Item *it, Item *args){
	Item *trace = args;
	while (trace != NULL){
		if (cmp(it->name, trace->name) == 0) return true;
		trace = trace->next;
	}
	return false;
}
//获取一个符号表项
Item *getItem(char *name){
	if (name == NULL) return NULL;
	Item *trace = table;
	while (trace != NULL){
		if ((cmp(trace->name, name) == 0) || (cmp(trace->type_name, name) == 0)) return trace;
		trace = trace->next;
	}
	return NULL;
}
//获取保存的类型，主要用在逗号隔开的定义那边
int getType(char *item){
	if (item == NULL) return -1;
	else {
		Item *it = (Item *)item;
		if (it->type == TYPE_FUNCTION) return it->ret_type;
		else return it->type;
	}
}
//设置函数参数个数
void setArgsNumber(char *item){
	if (item == NULL) return;
	else {
		Item *trace = table;
		((Item *)item)->args_num = 0;
		while (trace != NULL){
			if (trace->scope == (Item *)item && trace->type != TYPE_STRUCT) ((Item *)item)->args_num++;
			trace = trace->next;
		}
	}
}
//比较参数类型相等
bool cmpItem(Item *it1, Item *it2){
	if ((it1 == NULL && it2 != NULL) || (it1 != NULL && it2 == NULL)) return false;
	// 两个都是null，默认相等
	else if (it1 == NULL && it2 == NULL) return true;
	// 两个的类型相同，有两种情况（it1不可能出现function, if, else, while, struct类型, 只可能是var int, var float, var struct）
	else if (it1->type == it2->type && (TYPE_VAR_INT == it1->type || TYPE_VAR_FLOAT == it1->type || TYPE_VAR_STRUCT == it1->type)){
		// 如果恰好是结构体类型的变量
		if (it1->type == TYPE_VAR_STRUCT){
			// 结构体有名字且名字相同，默认相同
			if (cmp(it1->type_name, it2->type_name) == 0 && 0 < strlen(it1->type_name)) return true;
			// 结构体名字不同或者是匿名结构体，检查结构体成员
			// 这里忘记处理匿名结构体了。。。
			else {
				Item *list1 = getStructMember((char *)getItem(it1->type_name));
				Item *list2 = getStructMember((char *)getItem(it2->type_name));
				return cmpArgs(list1, list2);
			};
		}
		else return true;
	}
	else if ((it1->type == TYPE_VAR_INT && it2->type == TYPE_INT)||(it1->type == TYPE_VAR_FLOAT && it2->type == TYPE_FLOAT)) return true;
	else return false;
}
// 获取函数的参数列表
Item *getArgs(char *name){
//	displayTable(table);
	Item *result = NULL;
	Item *result_tail = NULL;
	Item *fun = getItem(name);
	if (fun != NULL && fun->type == TYPE_FUNCTION){
		// 参数总数
		int total = fun->args_num;
		Item *trace = table;
		while (trace != NULL && total > 0){
			// 前total个作用域是fun的变量就是函数的参数列表
			// 这一点对于struct的成员变量也是一样的
			if (trace->scope == fun && (trace->type == TYPE_VAR_INT || trace->type == TYPE_VAR_FLOAT || trace->type == TYPE_VAR_STRUCT)){
				if (result == NULL){
					result = newItem();
					memcpy(result, trace, sizeof(Item));
					result_tail = result;
					result_tail->next = NULL;
				}
				else {
					Item *temp = newItem();
					memcpy(temp, trace, sizeof(Item));
					result_tail->next = temp;
					result_tail = temp;
					result_tail->next = NULL;
				}
				total--;
			}
			trace = trace->next;
		}
		if (total > 0) printf("Error occur in get args\n");
	}
//	displayTable(table);
	return result;
}
//比较两个参数列表是否match
bool cmpArgs(Item *def, Item *in){
	while (def != NULL && in != NULL){
		// 参数类型不匹配
		if (!cmpItem(def, in)) return false;
		def = def->next;
		in = in->next;
	}
	// 参数个数不匹配
	if (def != NULL || in != NULL) return false;
	return true;
}
//获取所有的结构体成员
Item *getStructMember(char *item){
	Item *result = NULL;
	Item *tail = NULL;
	Item *it = (Item *)item;
	if (it == NULL || it->type != TYPE_STRUCT) return NULL;
	else {
		Item *trace = table;
		while (trace != NULL) {
			// 符号表里所有的作用域为结构体的变量就是结构体的成员列表
			if (trace->scope == it && (trace->type == TYPE_VAR_INT || trace->type == TYPE_VAR_FLOAT || trace->type == TYPE_VAR_STRUCT)){
				if (result == NULL){
					result = newItem();
					memcpy(result, trace, sizeof(Item));
					tail = result;
					tail->next = NULL;
				}
				else {
					Item *temp = newItem();
					memcpy(temp, trace, sizeof(Item));
					tail->next = temp;
					tail = temp;
					tail->next = NULL;
				}
			}
			trace = trace->next;
		}
	}
	return result;
}
//匿名结构体的结构体名
int anonymous = 0;
char *getAnonymousStruct(){
	char *name = (char *)malloc(ID_MAX_LEN*sizeof(char));
	sprintf(name, "%d_", anonymous++);
	return name;
}
//临时保存的类型
Item *temp_type = NULL;
//获取一个结构体所占的空间大小
int getStructSize(Item *it){
	if (it == NULL) return -1;
	if (TYPE_STRUCT == it->type || TYPE_VAR_STRUCT == it->type){
		int result = 0;
		Item *def = getItem(it->type_name);
		Item *mems = getStructMember((char *)def);
		Item *trace = mems;
		while (trace != NULL){
			int arrNum = getArrayNum(trace);
			if (arrNum == 0) arrNum = 1;
			switch(trace->type){
				case TYPE_VAR_INT: result += arrNum*4; break;
				case TYPE_VAR_FLOAT: result += arrNum*4; break;
				case TYPE_VAR_STRUCT: result += arrNum*getStructSize(trace); break;
				default: break;
			}
			trace = trace->next;
		}
		return result;
	}
	else return -1;
}
//获取一个类型所占的空间大小，注意只是类型，所以碰到数组要自己处理
int getTypeSize(Item *it){
	if (it == NULL) return -1;
	switch(it->type){
		case TYPE_VAR_INT: return 4;
		case TYPE_VAR_FLOAT: return 4;
		case TYPE_VAR_STRUCT: return getStructSize(it);
		default: return -1;
	}
}
//获取数组元素个数，注意多维数组
int getArrayNum(Item *it){
	if (it == NULL) return -1;
	int result = 0;
	if (0 >= it->dimension) return result;
	else if (NULL == it->dim_max)
		printf("Error occurred when getArrayNum, it->dim_max = NULL but dimension > 0\n");
	else {
		result = 1;
		int i;
		for (i = 0; i < it->dimension; i ++) result *= it->dim_max[i];
	}
	return result;
}
//临时保存类型
void saveTempType(Item *tp){
	if (NULL == temp_type) temp_type = newItem();
	if (tp != NULL) memcpy(temp_type, tp, sizeof(Item));
	else printf("Error occurred when saveTempType, tp is null\n");
}
//获取临时保存的类型
Item *getTempType(){
	return temp_type;
}
//生成临时变量
int temp_var_num = 0;
char *getTempVar(){
	char *name = (char *)malloc(ID_MAX_LEN*sizeof(char));
	memset(name, 0, ID_MAX_LEN);
	sprintf(name, "v%d", temp_var_num++);
	return name;
}
//假如是函数调用或者数组成员，需要做一些预处理
void printExp(Item **exp){
	if (NULL == exp) return;
	else {
		Item *e = *exp;
		if (NULL == e) return;
		else {
			if (e->dimension > 0){
				char *tvar = getTempVar();
				Midcode *code = newMidcode();
				if (e->name[0] == '*') sprintf(code->sentence, "%s := %s\n", tvar, e->name+1);
				else sprintf(code->sentence, "%s := &%s\n", tvar, e->name);
				code = newMidcode();
				sprintf(code->sentence, "%s := %s + %s\n", tvar, tvar, e->offset);
				memset(e->name, 0, ID_MAX_LEN);
				sprintf(e->name, "*%s", tvar);
				e->dimension = 0;
			}
			if (memcmp(e->name, "CALL", 4) == 0){
				char *tvar = getTempVar();
				Midcode *code = newMidcode();
				sprintf(code->sentence, "%s := %s\n", tvar, e->name);
				memset(e->name, 0, ID_MAX_LEN);
				sprintf(e->name, "%s", tvar);
			}
		}
	}
}
//初始化符号表，主要是加入write和read
void initTable(){
	Item *read = newItem();
	read->scope = NULL;
	read->args_num = 0;
	read->type = TYPE_FUNCTION;
	read->ret_type = TYPE_INT;
	cpy(read->name, "read");

	insertTable(read);

	Item *write = newItem();
	write->scope = NULL;
	write->args_num = 1;
	write->type = TYPE_FUNCTION;
	write->ret_type = TYPE_INT;
	cpy(write->name, "write");

	insertTable(write);

	Item *arg = newItem();
	arg->scope = write;
	arg->type = TYPE_VAR_INT;
	cpy(arg->name, "arg");
	arg->dimension = 0;

	insertTable(arg);
}

// 最后写入的文件的文件指针
FILE *file = NULL;

// 行号
int line_num = 0;

// 所有的中间代码保存这个链表里 
Midcode *codes = NULL;
Midcode *code_tail = NULL;

// label的number不会重复
int tag_num = 0;
// 一条中间代码
Midcode *newMidcode(){
	Midcode *code = (Midcode *)malloc(sizeof(Midcode));
	code->line = line_num ++;
	memset(code->sentence, 0, SENTENCE_MAX_LEN);
	code->next = NULL;

	if (codes == NULL) {
		codes = code;
		code_tail = code;
	}
	else {
		code_tail->next = code;
		code_tail = code;
	}
	return code;
}
// 一条待填充中间代码
codeItem *newcodeItem(){
	codeItem *item = (codeItem *)malloc(sizeof(codeItem));
	item->code = NULL;
	item->next = NULL;
	return item;
}

// 当前的M标号，存储的应该是LABEL M语句的M的名字
char *tagM = NULL;
void setM(char *m){
	if (tagM == NULL) tagM = (char *)malloc(ID_MAX_LEN);
	memset(tagM, 0, ID_MAX_LEN);
	sprintf(tagM, "%s", m);
	Midcode *code = newMidcode();
	sprintf(code->sentence, "LABEL %s :\n", m);
}
char *getM(){
	return tagM;
}

codeItem *mergeNode(codeItem *list, Midcode *st){
	codeItem *l = list;
	if (l == NULL) {
		l = newcodeItem();
		l->code = st;
		return l;
	}
	else{
		codeItem *item = newcodeItem();
		item->code = st;
		item->next = l;
		l = item;
		return l;
	}
}
codeItem *mergeList(codeItem *list1, codeItem *list2){
	codeItem *tail2 = list2;
	if (tail2 == NULL) return list1;
	else{
		while (tail2->next != NULL)
			tail2 = tail2->next;
		tail2->next = list1;
		return list2;
	}
}
// 回填的辅助函数
void replaceLabel(char *origin, char *label){
	int maxlen = strlen(origin);
	int i;
	for (i = 0; i < maxlen; i ++){
		if (origin[i] == '@') break;
	}
	if (i == maxlen) {
		printf("Error occurred in replace\n");
		return;
	}
	memset(origin+i, 0, SENTENCE_MAX_LEN-i);
	sprintf(origin+i, "%s\n", label);
}

void displaycodeItem(codeItem *list){
	codeItem *trace = list;
	while (trace != NULL){
		printf("%s", list->code->sentence);
		trace = trace->next;
	}
}

void backpatchList(codeItem *list, char *tag){
	//displaycodeItem(list);
	codeItem *trace = list;
	while (NULL != trace){
		replaceLabel(trace->code->sentence, tag);
		trace = trace->next;
	}
}

char *newTagName(){
	char *name = (char *)malloc(ID_MAX_LEN);
	memset(name, 0, ID_MAX_LEN);
	sprintf(name, "L_%d", tag_num++);
	return name;
}

void displayMidcode(){
	Midcode *trace = codes;
	while (NULL != trace){
		printf("%s", trace->sentence);
		trace = trace->next;
	}
}
// 保存中间代码
void storeMidcode(char *filename){
	file = fopen(filename, "w+");
	if (NULL == file){
		printf("Error occurred in open file\n");
		return;
	}
	Midcode *trace = codes;
	while (NULL != trace){
		fprintf(file, "%s", trace->sentence);
		trace = trace->next;
	}
	fclose(file);
}

void displayItemList(Item *it){
	printf("------truelist------\n");
	displaycodeItem(it->truelist);
	printf("------falselist------\n");
	displaycodeItem(it->falselist);
	printf("------nextlist------\n");
	displaycodeItem(it->nextlist);
}

int isBoolean = false;
int getBoolean(){
	return isBoolean;
}
void setBoolean(int b){
	isBoolean = b;
}
