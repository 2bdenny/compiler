#ifndef SDT_H_
#define SDT_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ID_MAX_LEN 33
#define bool int
#define true 1
#define TRUE 1
#define false 0
#define FALSE 0

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
	int type;	// 0:struct 1:function 2:int 3:float
	int line;
	struct item *curTable;	// A.cur = NULL
				// b.cur = B.cur = A
				// a.cur = C.cur = B
	struct item *subTable;	// A.sub = b.head; b.sub = NULL
				// B.sub = a.head
	struct item *next;	// b.next = B
	struct item *curTableTail;
} Item;

bool insertItem(Item *table, char *name, int type, int line);
void displayTable(int ct, Item *table);
#endif
