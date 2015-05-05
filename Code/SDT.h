#ifndef SDT_H_
#define SDT_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GrammarTree.h"
#define ID_MAX_LEN 33
#define bool int
#define true 1
#define TRUE 1
#define false 0
#define FALSE 0

// define type here
#define TYPE_STRUCT 0
#define TYPE_FUNCTION 3
#define TYPE_INT 1
#define TYPE_FLOAT 2

/*
   struct A{
   	int b;
   	struct B{
		int a;
		struct C{
			...
		}
	}
   }
 */
typedef struct item{
	char name[ID_MAX_LEN];
	int no;		// tno
	int type;	// 0:struct 3:function 1:int 2:float
	char type_name[ID_MAX_LEN];	// if struct, need struct name
	int line;
	int complete;	// 0:not 1:complete
	struct item *curTable;	// A.cur = NULL
				// b.cur = B.cur = A
				// a.cur = C.cur = B
	struct item *subTable;	// A.sub = b.head; b.sub = NULL
				// B.sub = a.head
	struct item *next;	// b.next = B
	struct item *curTableTail;
} Item;

Item *makeItem(Leaf *leaf);
bool insertItem(Item *table, Item *me);
void displayTable(int ct, Item *table);
void analysis(Leaf *tree, Item *table);
#endif
