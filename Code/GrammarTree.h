#ifndef GRAMMAR_TREE_H_
#define GRAMMAR_TREE_H_

#define LEAF_LEN sizeof(Leaf)
#define TREE_LEN sizeof(Tree)
#define true 1
#define false 0

typedef union value{
	double val_double;
	int val_int;
	float val_float;
	char val_name[32];
} Value;

// Tree
typedef struct tNode{
	int line;	// line number
	int valno;	// -1:null 0:double 1:int 2:float 3:type
	int terminal;	// 0:non-terminal 1:terminal
	char token[20];	// token name
	Value val;	// token val
	struct tNode *left;	// left child
	struct tNode *right;	// right brother
} Leaf;

// 制造一个叶子
Leaf *makeLeaf(int line, int valno, int terminal, char *token, Value val){
	Leaf *x = (Leaf *)malloc(sizeof(Leaf));
	x->line = line;
	x->valno = valno;
	x->terminal = terminal;
	memset(x->token, 0, 20);
	memcpy(x->token, token, strlen(token));
	x->val = val;
	x->left = NULL;
	x->right = NULL;
	return x;
}

// 限制：对于一个表达式，规约的时候，必须从左到右地增加child
int addChild(Leaf *parent, Leaf *child){
	if (parent->left == NULL) {
		parent->left = child;
		return true;
	}
	else {
		Leaf *brother = parent->left;
		while (brother->right != NULL) brother = brother->right;
		brother->right = child;
		return true;
	}
	return false;
}

// 展示一棵树
void display(Leaf *tree, int ntab){
	while (tree != NULL) {
		int i = 0;
		for (; i < ntab; i ++) printf("\t");
		if (tree->terminal == 0) printf("%s (%d)\n", tree->token, tree->line);
		else{
			printf("%s", tree->token);
			switch(tree->valno){
				case -1: printf("\n"); break;
				case 0: printf(": %f\n", tree->val.val_double); break;
				case 1: printf(": %d\n", tree->val.val_int); break;
				case 2: printf(": %f\n", tree->val.val_float); break;
				case 3: printf(": %s\n", tree->val.val_name); break;
			}
		}
		display(tree->left, ntab+1);
		display(tree->right, ntab);
	}
}

typedef struct fNode{
	Leaf *tree;	//这颗树
	struct fNode *next;	//下一颗树
} Tree;

extern Tree *forest = NULL;

// 在森林中增加一棵树
int addTree(Leaf *tree){
	Tree *tmp = (Tree *)malloc(sizeof(Tree));
	tmp->tree = tree;
	tmp->next = NULL;
	if (forest == NULL) {
		forest = tmp;
		return true;
	}
	else {
		tmp->next = forest;
		forest = tmp;
		return true;
	}
	return false;
}
// 删除一棵树
int delTree(Leaf *tree){
	Tree *tmp = forest;
	Tree *prev = NULL;
	if (tmp == NULL) return false;
	while (tmp->tree != tree) {
		if (tmp->next == NULL) return false;
		else {
			prev = tmp;
			tmp = tmp->next;
		}
	}
	if (prev == NULL) {
		forest = forest->next;
		return true;
	}
	else {
		prev->next = tmp->next;
		return true;
	}
	return false;
}
int reduce1(int ln, char *name, int l1){
	Leaf *l0 = makeLeaf(ln, ((Leaf *)l1)->valno, 0, name, ((Leaf *)l1)->val);
	addChild((Leaf *)l0, (Leaf *)l1);

	addTree((Leaf *)l0);
	delTree((Leaf *)l1);
	return (int)l0;
}
int reduce0(int ln, char *name){
	Value vl;
	vl.val_int = 0;
	Leaf *l0 = makeLeaf(ln, 0, 0, name, vl);
	addTree((Leaf *)l0);
	return (int)l0;
}
int reduce2(int ln, char *name, int l1, int l2){
	Leaf *l0 = makeLeaf(ln, ((Leaf *)l1)->valno, 0, name, ((Leaf *)l1)->val);
	addChild((Leaf *)l0, (Leaf *)l1);
	addChild((Leaf *)l0, (Leaf *)l2);

	addTree((Leaf *)l0);
	delTree((Leaf *)l1);
	delTree((Leaf *)l2);
	return (int)l0;
}
int reduce3(int ln, char *name, int l1, int l2, int l3){
	Leaf *l0 = makeLeaf(ln, ((Leaf *)l1)->valno, 0, name, ((Leaf *)l1)->val);
	addChild((Leaf *)l0, (Leaf *)l1);
	addChild((Leaf *)l0, (Leaf *)l2);
	addChild((Leaf *)l0, (Leaf *)l3);

	addTree((Leaf *)l0);
	delTree((Leaf *)l1);
	delTree((Leaf *)l2);
	delTree((Leaf *)l3);
	return (int)l0;
}
int reduce4(int ln, char *name, int l1, int l2, int l3, int l4){
	Leaf *l0 = makeLeaf(ln, ((Leaf *)l1)->valno, 0, name, ((Leaf *)l1)->val);
	addChild((Leaf *)l0, (Leaf *)l1);
	addChild((Leaf *)l0, (Leaf *)l2);
	addChild((Leaf *)l0, (Leaf *)l3);
	addChild((Leaf *)l0, (Leaf *)l4);

	addTree((Leaf *)l0);
	delTree((Leaf *)l1);
	delTree((Leaf *)l2);
	delTree((Leaf *)l3);
	delTree((Leaf *)l4);
	return (int)l0;
}
int reduce5(int ln, char *name, int l1, int l2, int l3, int l4, int l5){
	Leaf *l0 = makeLeaf(ln, ((Leaf *)l1)->valno, 0, name, ((Leaf *)l1)->val);
	addChild((Leaf *)l0, (Leaf *)l1);
	addChild((Leaf *)l0, (Leaf *)l2);
	addChild((Leaf *)l0, (Leaf *)l3);
	addChild((Leaf *)l0, (Leaf *)l4);
	addChild((Leaf *)l0, (Leaf *)l5);

	addTree((Leaf *)l0);
	delTree((Leaf *)l1);
	delTree((Leaf *)l2);
	delTree((Leaf *)l3);
	delTree((Leaf *)l4);
	delTree((Leaf *)l5);
	return (int)l0;
}
int reduce6(int ln, char *name, int l1, int l2, int l3, int l4, int l5, int l6){
	Leaf *l0 = makeLeaf(ln, ((Leaf *)l1)->valno, 0, name, ((Leaf *)l1)->val);
	addChild((Leaf *)l0, (Leaf *)l1);
	addChild((Leaf *)l0, (Leaf *)l2);
	addChild((Leaf *)l0, (Leaf *)l3);
	addChild((Leaf *)l0, (Leaf *)l4);
	addChild((Leaf *)l0, (Leaf *)l5);
	addChild((Leaf *)l0, (Leaf *)l6);

	addTree((Leaf *)l0);
	delTree((Leaf *)l1);
	delTree((Leaf *)l2);
	delTree((Leaf *)l3);
	delTree((Leaf *)l4);
	delTree((Leaf *)l5);
	delTree((Leaf *)l6);
	return (int)l0;
}
#endif
