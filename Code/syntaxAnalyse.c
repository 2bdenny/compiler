#include "syntaxAnalyse.h"
#include "GrammarTree.h"
#include "stack.h"
#define max(a, b) ((a)>(b)?(a):(b))

int number = 0;
char *cur_state = NULL;
Item *cur_scope = NULL;
Item *table = NULL;
Item *cur_item = NULL;
Item *tail = NULL;
int cmp(char *a, char *b){
	int maxlen = max(strlen(a), strlen(b));
	return memcmp(a, b, maxlen);
}
void *cpy(char *a, char *b){
	int len = strlen(b);
	return memcpy(a, b, len);
}
void initItem(Item *it){
	it->num = number++;
	it->scope = cur_scope;

	it->type = -1;
	memset(it->type_name, 0, ID_MAX_LEN);

	it->ret_type = -1;
	memset(it->ret_type_name, 0, ID_MAX_LEN);

	memset(it->name, 0, ID_MAX_LEN);
	it->line = -1;

	it->dimension = -1;
	it->dim_max = NULL;
	it->dim_step = NULL;

	it->next = NULL;
}
void deleteItem(Item *it){
	if (it->dim_max != NULL) free(it->dim_max);
	if (it->dim_step != NULL) free(it->dim_step);
	free(it);
}
void displayTable(Item *table){
	if (table == NULL) return;
	printf("num=%d, type=%d, type_name=%s, ret_type=%d, ret_type_name=%s, name=%s, line=%d\n", table->num, table->type, table->type_name, table->ret_type, table->ret_type_name, table->name, table->line);
	displayTable(table->next);
}
int getType(Leaf *t){
	if (cmp(t->val.val_name, "int") == 0) return TYPE_VAR_INT;
	if (cmp(t->val.val_name, "float") == 0) return TYPE_VAR_FLOAT;
}
void insertTable(Item *it){
	if (table == NULL) {
		tail = it;
		table = it;
	}
	else {
		tail->next = it;
		tail = it;
	}
}
bool contain(Item *it){
	if (it == NULL) return false;
	Item *trace = table;
	while (trace != NULL){
		if (trace->type == TYPE_STRUCT){
			if (it->type == TYPE_STRUCT) {
				if (cmp(trace->type_name, it->type_name) == 0) return true;
			}
			else {
				if (cmp(trace->type_name, it->name) == 0) return true;
			}
		}
		else {
			if (it->type == TYPE_STRUCT) {
				if (cmp(trace->name, it->type_name) == 0) return true;
			}
			else {
				if (cmp(trace->name, it->name) == 0) return true;
			}
		}
		trace = trace->next;
	}
	return false;
}
void analyse(Leaf *tree){
//	printf("START: tree=0x%lx, scope=0x%lx\n", tree, cur_scope);
//	if (tree != NULL) printf(" token=%s\n", tree->token);
	if (tree == NULL) return;
	else if (cmp(tree->token, "Program") == 0) {
	}
	else if (cmp(tree->token, "ExtDefList") == 0) {
	}
	else if (cmp(tree->token, "ExtDef") == 0) {
	}
	else if (cmp(tree->token, "Specifier") == 0){
	}
	else if (cmp(tree->token, "TYPE") == 0){
		cur_item = (Item *)malloc(sizeof(Item));
		initItem(cur_item);

		if (cmp(tree->val.val_name, "int") == 0) cur_item->type = TYPE_VAR_INT;
		else if (cmp(tree->val.val_name, "float") == 0) cur_item->type = TYPE_VAR_FLOAT;
	}
	else if (cmp(tree->token, "StructSpecifier") == 0){
	}
	else if (cmp(tree->token, "STRUCT") == 0){
		cur_item = (Item *)malloc(sizeof(Item));
		initItem(cur_item);

		cur_item->scope = cur_scope;
		cur_item->type = TYPE_STRUCT;
	}
	else if (cmp(tree->token, "OptTag") == 0){
	}
	else if (cmp(tree->token, "ID") == 0){
		// struct A a
		// struct A
		if (cur_item != NULL){
			if (cur_item->type == TYPE_STRUCT){
				// 结构体类型变量的定义
				if (strlen(cur_item->type_name) > 0) {
					// struct {int a;} c;
					// struct A{int b;} c;
					if (!contain(cur_item)) {
						insertTable(cur_item);

						cur_item = (Item *)malloc(sizeof(Item));
						initItem(cur_item);
						cur_item->scope = tail->scope;
						cur_item->type = TYPE_VAR_STRUCT;
						cpy(cur_item->type_name, tail->type_name);
						cpy(cur_item->name, tree->val.val_name);
						cur_item->line = tree->line;
						cur_item->dimension = 0;
					}
					// struct A c;
					else {
						Item *tmp = cur_item;

						cur_item = (Item *)malloc(sizeof(Item));
						initItem(cur_item);
						cur_item->scope = tmp->scope;
						cur_item->type = TYPE_VAR_STRUCT;
						cpy(cur_item->type_name, tmp->type_name);
						cpy(cur_item->name, tree->val.val_name);
						cur_item->line = tree->line;
						cur_item->dimension = 0;
					}
				}
				// 结构体定义
				else {
					cur_item->line = tree->line;
					cpy(cur_item->type_name, tree->val.val_name);

					if (!contain(cur_item))
						insertTable(cur_item);
				}
			}
			// 变量定义
			else if (cur_item->type == TYPE_VAR_INT || cur_item->type == TYPE_VAR_FLOAT){
				if (strlen(cur_item->name) == 0){
					cur_item->line = tree->line;
					cpy(cur_item->name, tree->val.val_name);
					cur_item->dimension = 0;
				} 
			}
		}
	}
	// 函数或结构体内部定义开始
	else if (cmp(tree->token, "LC") == 0){
		if (cur_item != NULL){
			if (cur_item->type == TYPE_STRUCT) {
				if (!contain(cur_item)) insertTable(cur_item);
				cur_scope = cur_item;
			}
			// 结构体内部定义开始
			// struct {}; 这种结构体没有名字的情况
			if (cur_item->type == TYPE_VAR_STRUCT && !contain(cur_item)){
				cur_item->line = tree->line;
				insertTable(cur_item);

				cur_item = cur_item->scope;
			}
		}
	}
	else if (cmp(tree->token, "DefList") == 0){
	}
	else if (cmp(tree->token, "Def") == 0){
	}
	else if (cmp(tree->token, "DecList") == 0){
	}
	else if (cmp(tree->token, "Dec") == 0){
	}
	else if (cmp(tree->token, "VarDec") == 0){
	}
	// 函数或结构体内部定义结束
	else if (cmp(tree->token, "RC") == 0){
		// 函数定义结束
/*		if (cur_item == NULL) cur_scope = NULL;
		// 结构体定义结束
		else if (cur_item->scope != NULL && cur_item->scope->type == TYPE_STRUCT) {
			cur_scope = cur_item->scope;
			cur_item = cur_scope;
		}*/
		cur_scope = tail->scope;
		cur_item = cur_scope;
	}
	else if (cmp(tree->token, "Tag") == 0){
	}
	else if (cmp(tree->token, "SEMI") == 0){
//		cur_item = cur_item->scope;
		// struct {int b;};
		// int ;
		if (cur_item != NULL){
			if (cur_item->type == TYPE_STRUCT){
				if (!contain(cur_item)){
					insertTable(cur_item);
					cur_item = cur_item->scope;
				}
			}
			else if (cur_item->type == TYPE_VAR_INT || cur_item->type == TYPE_VAR_FLOAT || cur_item->type == TYPE_VAR_STRUCT){
				if (!contain(cur_item)){
					insertTable(cur_item);
					cur_item = cur_item->scope;
				}
			}
		}
	}
	else if (cmp(tree->token, "COMMA") == 0){
		Item *tmp = cur_item;
		insertTable(cur_item);

		cur_item = (Item *)malloc(sizeof(Item));
		initItem(cur_item);
		cur_item->scope = tmp->scope;
		cur_item->type = tmp->type;
		cpy(cur_item->type_name, tmp->type_name);
	}
	else if (cmp(tree->token, "LB") == 0){
		cur_item->dimension ++;

		if (cur_item->dim_max != NULL){
			int *prev = cur_item->dim_max;
			cur_item->dim_max = (int *)malloc(cur_item->dimension);
			int i;
			for (i = 0; i < cur_item->dimension-1; i ++) cur_item->dim_max[i] = prev[i];
			free(prev);

			cur_item->dim_max[cur_item->dimension-1] = -1;
		}
		else {
			cur_item->dim_max = (int *)malloc(cur_item->dimension);
			cur_item->dim_max[cur_item->dimension-1] = -1;
		}
	}
	else if (cmp(tree->token, "INT") == 0){
		if (cur_item != NULL){
			if (cur_item->dim_max != NULL && cur_item->dim_max[cur_item->dimension-1] == -1)
				cur_item->dim_max[cur_item->dimension-1] = tree->val.val_int;
		}
	}
	else if (cmp(tree->token, "LP") == 0){
		cur_item->ret_type = cur_item->type;
		cpy(cur_item->ret_type_name, cur_item->type_name);
		cur_item->type = TYPE_FUNCTION;
		memset(cur_item->type_name, 0, ID_MAX_LEN);

		insertTable(cur_item);
		cur_scope = cur_item;
	}
//	printf("END: tree=0x%lx, scope=0x%lx\n", tree, cur_scope);
//	if (tree != NULL) printf(" token=%s\n", tree->token);
//	if (cur_item != NULL) printf("addr=%lx, scope=%lx, type_name=%s, ret_type_name=%s, name=%s\n", cur_item, cur_item->scope, cur_item->type_name, cur_item->ret_type_name, cur_item->name);
	analyse(tree->left);
	analyse(tree->right);
}
