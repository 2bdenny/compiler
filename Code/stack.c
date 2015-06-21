#include "stack.h"

sNode *tableStack = NULL;
int stackSize = 0;

bool push(int x){
	if (isFull()){
		printf("stack already Full\n");
		return false;
	}
	else{
		stackSize++;
		sNode *tmp = (sNode *)malloc(sizeof(sNode));
//		memset(tmp->content, 0, CONTENT_LEN);
//		memcpy(tmp->content, x, strlen(x));
		tmp->content = x;
		tmp->next = tableStack;
		tableStack = tmp;
		return true;
	}
}
bool pop(){
	if (isEmpty()){
		printf("stack already NULL\n");
		return false;
	}
	else{
		stackSize--;
		sNode *tmp = tableStack;
		tableStack = tableStack->next;
		free(tmp);
		return true;
	}
}

int top(){
	if (isEmpty()){
		printf("stack is NULL\n");
		return -1;
	}
	else{
//		char *result = (char *)malloc(strlen(tableStack->content)+1);
//		memset(result, 0, strlen(tableStack->content)+1);
//		memcpy(result, tableStack->content, strlen(tableStack->content));
		return tableStack->content;
	}
}

bool isEmpty(){
	return stackSize == 0? true:false;
}
bool isFull(){
	return stackSize == MAX_SIZE? true:false;
}
int size(){
	return stackSize;
}
void displayStack(){
	int i;
	sNode *trace = tableStack;
	for (i = stackSize; i > 0; i --){
		printf("%d: %lx\n", i, (unsigned long)trace->content);
		trace = trace->next;
	}
}

void clear(){
	while (stackSize > 0){
		pop();
		stackSize --;
	}
}
