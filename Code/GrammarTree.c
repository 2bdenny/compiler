#include "GrammarTree.h"
#include <stdlib.h>
#include <stdio.h>
Tree *forest = NULL;

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
//	printf("Leaf %ld\n", (long)x);
	return x;
}

// 限制：对于一个表达式，规约的时候，必须从左到右地增加child
int addChild(Leaf *parent, Leaf *child){
//	printf("parent %ld; left %ld; right %ld\n", (long)parent, (long)parent->left, (long)parent->right);

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
	if (tree != NULL) {
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


// 在森林中增加一棵树
int addTree(Leaf *tree){
//	treeCount ++;
//	printf("add tree %ld\n", (long)forest);
	if (forest == NULL) {
		forest = (Tree *)malloc(TREE_LEN);
		forest->tree = tree;
		forest->next = NULL;
		return true;
	}
	else {
		Tree *tmp = (Tree *)malloc(TREE_LEN);
		tmp->tree = tree;
		tmp->next = forest;
		forest = tmp;
		return true;
	}
	return false;
}
// 删除一棵树
int delTree(Leaf *tree){
//	printf("del tree %ld\n", (long)tree);
	Tree *prev = NULL;
	Tree *tmp = forest;
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
void displayForest(Tree *f){
	while (f != NULL){
		printf("%s\n", (f->tree)->token);
		f = f->next;
	}
}
