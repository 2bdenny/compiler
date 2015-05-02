#ifndef STACK_H_
#define STACK_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define false 0
#define true 1
#define bool int
#define FALSE 0
#define TRUE 1
#define MAX_SIZE 65535
#define CONTENT_LEN 33

typedef struct stackNode{
	void *content;
	struct stackNode *next;
} sNode;

bool push(void *x);
bool pop();
void *top();
bool isEmpty();
bool isFull();
int size();
void display();
void clear();

#endif
