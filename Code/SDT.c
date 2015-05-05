#include "SDT.h"
#include "GrammarTree.h"

int number = 0;
Item *symbolTable = NULL;

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
			//////////////////////////start here
		}
	}

	return tmp;
}

// 插入到符号表
bool insertItem(Item *table, Item *me){
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
}


void analysis(Leaf *tree, Item *table){
	Item *it = makeItem(tree);
	Item *subtable = getTable(table);

	if (it != NULL) insertTable(table, it);
	if (tree->left != NULL) analysis(tree->left, subtable);
	if (tree->right != NULL) analysis(tree->right, table);
	return ;
}
