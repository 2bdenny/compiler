#ifndef GRAMMAR_TREE_H_
#define GRAMMAR_TREE_H_

#include <stdlib.h>
#include <string.h>

// 定义语法树常量
#define LEAF_LEN sizeof(struct tNode)
#define TREE_LEN sizeof(struct fNode)

// 定义bool类型
#define bool int
#define false 0
#define true 1

// 定义词法分析类型常量valno
#define VAL_INT 1
#define VAL_FLOAT 2
#define VAL_TYPE 3
#define VAL_NULL -1
#define VAL_DOUBLE 0

// ID最长是33字节
#define ID_MAX_LEN 33

// 定义语义分析常量type
#define TYPE_IF 0
#define TYPE_ELSE 1
#define TYPE_INT 2
#define TYPE_FLOAT 3
#define TYPE_VAR_INT 4
#define TYPE_VAR_FLOAT 5
#define TYPE_VAR_STRUCT 6
#define TYPE_STRUCT 7
#define TYPE_FUNCTION 8
#define TYPE_WHILE 9

// 宏定义函数
#define max(a, b) ((a) > (b)? (a):(b))

// 词法分析保存值/变量名
struct value{
	double val_double;
	int val_int;
	float val_float;
	char val_name[40];
};
typedef struct value Value;

// 语法树节点
struct tNode{
	int line;	// line number
	int valno;	// -1:null 0:double 1:int 2:float 3:type
	int terminal;	// 0:non-terminal 1:terminal
	char token[40];	// token name
	Value val;	// token val
	struct tNode *left;	// left child
	struct tNode *right;	// right brother
};
typedef struct tNode Leaf;

// 语法树
struct fNode{
	Leaf *tree;	//这颗树
	struct fNode *next;	//下一颗树
};
typedef struct fNode Tree;

// 符号表项
typedef struct item{
	struct item *scope;
	int number;
	int args_num;

	int type;
	char type_name[ID_MAX_LEN];

	int ret_type;
	char ret_type_name[ID_MAX_LEN];

	int result_type;
	char result_type_name[ID_MAX_LEN];

	char name[ID_MAX_LEN];
	int line;

	int dimension;
	int *dim_max;

	struct item *next;
}Item;

// 制造一个叶子
Leaf *makeLeaf(int line, int valno, int terminal, char *token, Value val);

// 限制：对于一个表达式，规约的时候，必须从左到右地增加child
int addChild(Leaf *parent, Leaf *child);

// 展示一棵子树
void display(Leaf *tree, int ntab);

// 删除一棵树
void destroy(Leaf **tree);

// 删除所有的树
void destroyForest();

// 展示一棵树
void displayTree(Tree *f);

// 在森林中增加一棵树
int addTree(Leaf *tree);

// 删除一棵树
int delTree(Leaf *tree);

// 用这个让错误恢复之后不输出语法树
void meetError();

// memcpy和memcmp的简写
void *cpy(char *a, char *b);
int cmp(char *a, char *b);

// 获取当前scope
Item *getScope();

// 设置当前scope
void setScope(Item *new_scope);

// 生成一个新符号表项
Item *newItem();

// 将符号表项插入到符号表
void insertTable(Item *x);

// 输出符号表
void displayTable(Item *table);

// 符号表里是否包含以var为名的变量或者结构体
bool isContain(char *var);

// 获取符号表里以name为名的符号表项
Item *getItem(char *name);

// 获取符号表项item的类型
int getType(char *item);

// 设置函数类型符号表项的参数个数
void setArgsNumber(char *item);

// 获取name为名的函数的参数列表
Item *getArgs(char *name);

// 获取item这个结构体的所有成员
Item *getStructMember(char *item);

// 比较两个参数列表是否相等
bool cmpArgs(Item *def, Item *in);

// 比较两个参数是否相等
bool cmpItem(Item *it1, Item *it2);

char *getAnonymousStruct();
#endif
