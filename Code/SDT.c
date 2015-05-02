#include "SDT.h"

int number = 0;
Item *symbolTable = NULL;

bool insertItem(Item *table, char *name, int type, int line){
	Item *tmp = (Item *)malloc(sizeof(Item));
	memset(tmp->name, 0, ID_MAX_LEN);
	memcpy(tmp->name, name, strlen(name));
	tmp->no = number++;
	tmp->type = type;
	tmp->line = line;
	tmp->curTable = table;
	tmp->subTable = NULL;
	tmp->next = NULL;
	tmp->curTableTail = NULL;

	if (table == NULL){
		symbolTable = tmp;
		symbolTable->curTableTail = tmp;
		return true;
	}
	else {
		if (table->curTableTail == NULL){
			table->subTable = tmp;
			table->curTableTail = tmp;
		}
		else {
			table->curTableTail->next = tmp;
			table->curTableTail = tmp;
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
	if (table->next != NULL) display(ct, table->next);
}
