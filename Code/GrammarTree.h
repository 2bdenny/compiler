#ifndef GRAMMAR_TREE_H_
#define GRAMMAR_TREE_H_

#define LEAF_LEN sizeof(Leaf)
#define TREE_LEN sizeof(Tree)

typedef union value{
	double val_double;
	int val_int;
	float val_float;
	char name[32];
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
	x->terminal = termial;
	memset(x->token, 0, 20);
	memcpy(x->token, token, strlen(token));
	memcpy(x->val, val, sizeof(Value));
	x.left = NULL;
	x.right = NULL;
	return x;
}

// 限制：对于一个表达式，规约的时候，必须从左到右地增加child
bool addChild(Leaf *parent, Leaf *child){
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
		if (terminal == 0) printf("%s (%d)\n", cur->token, cur->line);
		else{
			printf("%s", cur->token);
			switch(valno){
				case -1: printf("\n"); break;
				case 0: printf(": %f\n", val.val_double); break;
				case 1: printf(": %d\n", val.val_int); break;
				case 2: printf(": %f\n", val.val_float); break;
				case 3: printf(": %s\n", val.val_name); break;
			}
		}
		display(cur->left, ntab+1);
		display(cur->right, ntab);
	}
}

typedef struct fNode{
	Leaf *tree;	//这颗树
	fNode *next;	//下一颗树
} Tree;

extern Tree *forest = NULL;

// 在森林中增加一棵树
bool addTree(Leaf *tree){
	if (forest == NULL) {
		forest = tree;
		return true;
	}
	else {
		Tree *tmp = forest;
		while (tmp->next != NULL) tmp = tmp->next;
		tmp->next = tree;
		return true;
	}
	return false;
}
// 删除一棵树
bool delTree(Leaf *tree){
	Tree *prev = NULL;
	Tree *tmp = forest;
	while (tmp->tree != tree) {
		if (tmp->next == NULL) return false;
		else {
			prev = tmp;
			tmp = tmp->next;
		}
	}
	if (prev == NULL) {
		forest = forest->next;
		free(tmp);
		return true;
	}
	else {
		prev->next = tmp->next;
		free(tmp);
		return true;
	}
	return false;
}
#endif
