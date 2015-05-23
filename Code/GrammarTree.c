#include "GrammarTree.h"
#include <stdlib.h>
#include <stdio.h>
Item *table = NULL;
Item *tail = NULL;
Item *scope = NULL;
int count = 0;

Tree *forest = NULL;
int getError = 0;

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
//	printf("Leaf %ld\n", (long)x);
	return x;
}

// 限制：对于一个表达式，规约的时候，必须从左到右地增加child
int addChild(Leaf *parent, Leaf *child){
//	printf("parent %ld; left %ld; right %ld\n", (long)parent, (long)parent->left, (long)parent->right);

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
//	treeCount ++;
//	printf("add tree %ld\n", (long)forest);
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
//	printf("del tree %s\n", tree->token);
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
	//display(*tree, 0);
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

int cmp(char *a, char *b){
	int maxLen = max(strlen(a), strlen(b));
	return memcmp(a, b, maxLen);
}
void *cpy(char *a, char *b){
	int maxLen = max(strlen(a), strlen(b));
	return memcpy(a, b, maxLen);
}
Item *getScope(){return scope;}
void setScope(Item *new_scope){scope = new_scope;}
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

	return result;
}
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
//		printf("insert %s now\n", x->name);
		x->next = NULL;
		if (table == NULL) {
			table = x;
			tail = x;
		}
		else {
//				printf("tail=%lx\n", tail);
			tail->next = x;
			tail = x;
//			if (cmp(x->name, "temp") == 0) {
//				printf("tail=%lx\n", tail);
//				displayTable(table);
//			}
		}
	}
//	displayTable(table);
}
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

Item *getItem(char *name){
	if (name == NULL) return NULL;
	Item *trace = table;
	while (trace != NULL){
		if ((cmp(trace->name, name) == 0) || (cmp(trace->type_name, name) == 0)) return trace;
		trace = trace->next;
	}
	return NULL;
}
int getType(char *item){
	if (item == NULL) return -1;
	else {
		Item *it = (Item *)item;
		if (it->type == TYPE_FUNCTION) return it->ret_type;
		else return it->type;
	}
}
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
//		printf("total=%d\n", total);
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
//	printf("函数的参数列表是:\n");
//	displayTable(table);
	return result;
}
bool cmpArgs(Item *def, Item *in){
//	printf("------start cmp args------\n");
//	printf("------def is------\n");
//	displayTable(def);
//	printf("------in is------\n");
//	displayTable(in);
//	printf("--------------------------\n");
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

int anonymous = 0;
char *getAnonymousStruct(){
	char *name = (char *)malloc(ID_MAX_LEN*sizeof(char));
	sprintf(name, "%d_", anonymous++);
	return name;
}


Item *temp_type = NULL;
int getStructSize(Item *it){
	if (it == NULL) return -1;
	if (TYPE_STRUCT == it->type || TYPE_VAR_STRUCT == it->type){
		int result = 0;
		Item *def = getItem(it->type_name);
		Item *mems = getStructMember((char *)def);
		Item *trace = mems;
		while (trace != NULL){
			switch(trace->type){
				case TYPE_VAR_INT: result += 4; break;
				case TYPE_VAR_FLOAT: result += 4; break;
				case TYPE_VAR_STRUCT: result += getStructSize(trace); break;
				default: break;
			}
			trace = trace->next;
		}
		return result;
	}
	else return -1;
}
int getTypeSize(Item *it){
	if (it == NULL) return -1;
	switch(it->type){
		case TYPE_VAR_INT: return 4;
		case TYPE_VAR_FLOAT: return 4;
		case TYPE_VAR_STRUCT: return getStructSize(it);
		default: return -1;
	}
}
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
void saveTempType(Item *tp){
	if (NULL == temp_type) temp_type = newItem();
	if (tp != NULL) memcpy(temp_type, tp, sizeof(Item));
	else printf("Error occurred when saveTempType, tp is null\n");
}
Item *getTempType(){
	return temp_type;
}

int temp_var_num = 0;
char *getTempVar(){
	char *name = (char *)malloc(ID_MAX_LEN*sizeof(char));
	memset(name, 0, ID_MAX_LEN);
	sprintf(name, "v%d", temp_var_num++);
	return name;
}
void printExp(Item **exp){
	if (NULL == exp) return;
	else {
		Item *e = *exp;
		if (NULL == e) return;
		else {
			if (e->dimension > 0){
				char *tvar = getTempVar();
				printf("%s := &%s\n", tvar, e->name);
				printf("%s := %s + %s\n", tvar, tvar, e->offset);
				printf("%s := *%s\n", tvar, tvar);
				memset(e->name, 0, ID_MAX_LEN);
				sprintf(e->name, "%s", tvar);
				e->dimension = 0;
			}
		}
	}
}
