#include "SDT.h"
#include "GrammarTree.h"
#define max(a, b) ((a)>(b)?(a):(b))

int number = 0;
char *cur_state = NULL;
Item *cur_scope = NULL;
Item *table = NULL;
Item *cur_item = NULL;
Item *tail_item = NULL;
/*
// 语法树的一个节点->符号表的一个项
Item *makeItem(Leaf *leaf, Item *tmp){
	// 如果接下来是一个全新的符号
	if (tmp == NULL) {
		tmp = (Item *)malloc(sizeof(Item));
		memset(tmp->type_name, 0, ID_MAX_LEN);
		memset(tmp->name, 0, ID_MAX_LEN);
		tmp->type = -1;		// 类型标志初始化为-1，并不清楚是什么类型
		tmp->no = number++;
		tmp->curTable = NULL;
		tmp->subTable = NULL;
		tmp->next = NULL;
		tmp->curTableTail = NULL;
	}

	// 否则tmp表示当前正在处理的符号
	// 如果当前正在处理的是一个终结符，直接保存信息
	if (leaf->terminal == 1){
		// 如果是TYPE或者ID，这里TYPE只可能是int或者float
		if (leaf->valno == 3){
			// 如果是ID，记录行数、变量名
			if (memcmp(leaf->token, "ID", sizeof("ID")) == 0){
				tmp->line = leaf->line;
				// 如果是struct的名字
				if (tmp->type == TYPE_STRUCT){
					memcpy(tmp->type_name, leaf->val.val_name, sizeof(leaf->val.val_name));
				}
				// 如果是变量名/函数名
				else {
					memcpy(tmp->name, leaf->val.val_name, strlen(leaf->val.val_name));
				}
			}
			// 如果是TYPE，记录TYPE类型
			else if (memcmp(leaf->token, "TYPE", sizeof("TYPE")) == 0){
				// 如果TYPE是int
				if (memcmp(leaf->val.val_name, "int", sizeof("int")) == 0) tmp->type = TYPE_INT;
				// 如果TYPE是float
				if (memcmp(leaf->val.val_name, "float", sizeof("float")) == 0) tmp->type = TYPE_FLOAT;
			}
		}
		// 是终结符，并且valno=-1，是关键字，只需要关注其中的struct
		else if (leaf->valno == -1){
			if (memcmp(leaf->token, "STRUCT", sizeof("STRUCT")) == 0){
				tmp->type = TYPE_STRUCT;
			}
		}
	}

	return tmp;
}*/

// 插入到符号表
/*bool insertItem(Item *table, Item *me){
	if (table == NULL){
		symbolTable = me;
		symbolTable->curTableTail = me;
		return true;
	}
	else {
		if (table->curTableTail == NULL){
			table->subTable = me;
			table->curTableTail = me;
		}
		else {
			table->curTableTail->next = me;
			table->curTableTail = me;
		}
		return true;
	}
	return false;
}

void displayTable(int ct, Item *table){
	int i;
	for (i = 0; i < ct; i ++) printf("  ");
	printf("%d: %s\n", table->no, table->name);
	if (table->subTable != NULL) displayTable(ct+1, table->subTable);
	if (table->next != NULL) displayTable(ct, table->next);
}*/

int cmp(char *a, char *b){
	int maxlen = max(strlen(a), strlen(b));
	return memcmp(a, b, maxlen);
}
int cpy(char *a, char *b){
	int len = strlen(b);
	return memcpy(a, b, len);
}

void analysis(Leaf *tree){
	if (tree == NULL) return;
	else {	// if new definition
		if (cur_item == NULL){
			cur_item = (Item *)malloc(sizeof(Item));
			cur_item->num = number++;
			cur_item->scope = cur_scope;
			cur_item->ret_type = -1;
			memset(cur_item->ret_type_name, 0, ID_MAX_LEN);
			cur_item->type = -1;
			memset(cur_item->type_name, 0, ID_MAX_LEN);
			memset(cur_item->name, 0, ID_MAX_LEN);
			cur_item->line = -1;
			cur_item->dimension = -1;
			cur_item->dim_max = NULL;
			cur_item->dim_step = NULL;
			cur_item->next = NULL;

			if (table == NULL) {
				table = cur_item;
				tail_item = cur_item;
			}
			else {
				tail_item->next = cur_item;
				tail_item = cur_item;
			}
		}

		// int/float
		if (cmp(tree->token, "TYPE") == 0){
			if (cmp(tree->val.val_name, "int") == 0) cur_item->type = TYPE_VAR_INT;
			else if (cmp(tree->val.val_name, "float") == 0) cur_item->type = TYPE_VAR_FLOAT;
		}
		// struct
		else if (cmp(tree->token, "STRUCT") == 0){
			cur_item->type = TYPE_VAR_STRUCT;
		}
		// id
		else if (cmp(tree->token, "ID") == 0){
			if (cur_item->type == -1) ;//add checker here
			else if (cur_item->type = TYPE_VAR_STRUCT){
				cpy(cur_item->type_name, tree->val.val_name);
			}
			else if (cur_item->type = TYPE_VAR_INT){
				cpy(cur_item->name, tree->val.val_name);
			}
			else if (cur_item->type = TYPE_VAR_FLOAT){
				cpy(cur_item->name, tree->val.val_name);
			}
		}
		// function
		else if (cmp(tree->token, "LP") == 0){
			// function end
			if (cur_item->type == TYPE_VAR_INT || cur_item->type == TYPE_VAR_FLOAT || cur_item->type == TYPE_VAR_STRUCT){
				cur_item->ret_type = cur_item->type;
				cpy(cur_item->ret_type_name, cur_item->type_name);
				memset(cur_item->type_name, 0, ID_MAX_LEN);

				// function inside
				cur_scope = cur_item;
				cur_item = NULL;
			}
		}
		// comma int a, b<-
		else if (cmp(tree->token, "COMMA") == 0){
			Item *tmp = (Item *)malloc(sizeof(Item));
			tmp->num = number++;
			tmp->scope = cur_scope;
			tmp->ret_type = -1;
			memset(tmp->ret_type_name, 0, ID_MAX_LEN);
			tmp->type = cur_item->type;
			cpy(tmp->type_name, cur_item->type_name);
			memset(tmp->name, 0, ID_MAX_LEN);
			tmp->line = -1;
			tmp->dimension = -1;
			tmp->dim_max = NULL;
			tmp->dim_step = NULL;
			tmp->next = NULL;

			cur_item = tmp;
			tail_item->next = cur_item;
			tail_item = cur_item;
		}
		else if (cmp(tree->token, "LB") == 0){
		}

		analysis(tree->left);
		analysis(tree->right);
	}
}
