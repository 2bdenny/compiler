#ifndef GRAMMAR_TREE_H_
#define GRAMMAR_TREE_H_

#include <stdlib.h>
#include <string.h>
#define LEAF_LEN sizeof(struct tNode)
#define TREE_LEN sizeof(struct fNode)
#define bool int
#define false 0
#define true 1
#define VAL_INT 1
#define VAL_FLOAT 2
#define VAL_TYPE 3
#define VAL_NULL -1
#define VAL_DOUBLE 0
#define ID_MAX_LEN 33
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
#define max(a, b) ((a) > (b)? (a):(b))

struct value{
	double val_double;
	int val_int;
	float val_float;
	char val_name[40];
};
typedef struct value Value;
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
struct fNode{
	Leaf *tree;	//这颗树
	struct fNode *next;	//下一颗树
};
typedef struct fNode Tree;

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
// 展示一棵树
void display(Leaf *tree, int ntab);
void destroy(Leaf **tree);
void destroyForest();
void displayTree(Tree *f);
// 在森林中增加一棵树
int addTree(Leaf *tree);
// 删除一棵树
int delTree(Leaf *tree);

void meetError();

void *cpy(char *a, char *b);
int cmp(char *a, char *b);

Item *getScope();
void setScope(Item *new_scope);

Item *newItem();
void insertTable(Item *x);
void displayTable(Item *table);
bool isContain(char *var);
Item *getItem(char *name);
int getType(char *item);
void setArgsNumber(char *item);
Item *getArgs(char *name);
bool cmpArgs(Item *def, Item *in);
#endif
