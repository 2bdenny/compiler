#ifndef GRAMMAR_TREE_H_
#define GRAMMAR_TREE_H_

#include <stdlib.h>
#include <string.h>
#define LEAF_LEN sizeof(struct tNode)
#define TREE_LEN sizeof(struct fNode)
#define false 0
#define true 1

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
#endif
