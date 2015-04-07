struct tNode{
	int line;
	int terminal;	// 0:non-terminal 1:terminal
	char token[20];	// token name
	char text[100];	// token val
	tNode *left;	// left child
	tNode *right;	// right brother
};

extern tNode *tree = NULL;

struct tNode *makeNode(int line, int terminal, char *token, char *text){
	struct tNode *x = (struct tNode *)malloc(sizeof(struct tNode));
	x.line = line;
	x.terminal = termial;
	memset(x.token, 0, 20);
	memcpy(x.token, token, strlen(token));
	memset(x.text, 0, 100);
	memcpy(x.text, text, strlen(text));
	x.left = NULL;
	x.right = NULL;
	return x;
}

bool addNode(struct tNode *parent, struct tNode *child){
	return false;
}

void display(struct tNode *cur, int ntab){
	while (cur != NULL) {
		int i = 0;
		for (; i < ntab; i ++) printf("\t");
		printf("%s (%d)\n", cur->token, cur->line);
		display(cur->left, ntab+1);
		display(cur->right, ntab);
	}
}
