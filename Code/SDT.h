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
#define TYPE_VAR_FLOAT 2
#define TYPE_VAR_INT 1
#define TYPE_VAR_STRUCT 4

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
	int num;			//标号
	struct item *scope;		//作用域，如果是函数/全局=NULL

	int type;			//变量类型
	char type_name[ID_MAX_LEN];	//类型名，用于TYPE_VAR_STRUCT

	int ret_type;			//返回值类型
	char ret_type_name[ID_MAX_LEN];	//返回值类型名

	char name[ID_MAX_LEN];		//变量名/函数名
	int line;			//行号

	int dimension;			//数组维数，用于数组类型变量
	int *dim_max;			//数据每一维的最大值
	int *dim_step;			//每一维所占空间

	struct item *next;		//下一个表项
} Item;


//Item *makeItem(Leaf *leaf, Item* tmp);
//bool insertItem(Item *table, Item *me);
//void displayTable(int ct, Item *table);
void analysis(Leaf *tree);
#endif
