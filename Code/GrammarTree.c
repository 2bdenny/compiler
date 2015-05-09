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
		else if (x->type != TYPE_STRUCT && isContain(x->name)){
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
void displayTable(Item *table){
	printf("--------Current Table---------\n");
	int number = -1;
	Item *trace = table;
	while (trace != NULL && trace->number > number){
		printf("%lx:%d Item:\tnum=%d scope=%lx type=%d type_name=%s \n\tret_type=%d ret_type_name=%s name=%s line=%d\n", trace, trace->line, trace->number, trace->scope, trace->type, trace->type_name, trace->ret_type, trace->ret_type_name, trace->name, trace->line);
		printf("\n");
		number = trace->number;
		trace = trace->next;
	}
	printf("------------------------------\n");
}

bool isContain(char *var){
	if (var == NULL) return false;
	Item *trace = table;
	while (trace != NULL){
		if (cmp(trace->name, var) == 0) return true;
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
			if (trace->scope == (Item *)item) ((Item *)item)->args_num++;
			trace = trace->next;
		}
	}
}
Item *getArgs(char *name){
	Item *result = NULL;
	Item *result_tail = NULL;
	Item *fun = getItem(name);
	if (fun != NULL && fun->type == TYPE_FUNCTION){
		int total = fun->args_num;
		Item *trace = table;
		while (trace != NULL && total > 0){
			if (trace->scope == fun){
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
				total--;
			}
			trace = trace->next;
		}
		if (total > 0) printf("Error occur in get args\n");
	}
	return result;
}
bool cmpArgs(Item *def, Item *in){
	while (def != NULL && in != NULL){
		if (def->type == in->type){
			if (!(def->type == TYPE_VAR_STRUCT && (cmp(def->type_name, in->type_name) == 0))) return false;
			def = def->next;
			in = in->next;
		}
		else return false;
	}
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
			if (trace->scope == it){
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
