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

// 下面的函数都是用于处理跳转相关语句的
// 为了保存行号，先把所有代码放在内存里，使用列表存起来【虽然比较慢
// 本来是直接输出到文件的，后来觉得文件操作太麻烦请教了余子洵，然后get了这个方法

#define SENTENCE_MAX_LEN 100

// 中间代码一条语句的结构体
typedef struct midcode {
	int line;		//行号
	char sentence[SENTENCE_MAX_LEN];	//中间代码
	struct midcode *next;	//下一条语句
} Midcode;
// 每一条语句都保证唯一，绝对不会有一条语句的多个副本，因此都是指针操作【好深的坑

// 这个结构体用于保存truelist和falselist
typedef struct codeitem{
	Midcode *code;		//中间代码的地址
	struct codeitem *next;	//下一个list的节点
} codeItem;
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
	char offset[ID_MAX_LEN*2];
	char def_name[ID_MAX_LEN];

	codeItem *truelist;
	codeItem *falselist;
	codeItem *nextlist;

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

// 判断是不是参数
bool isParameter(Item *it, Item *args);

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

//获取类型的空间大小
int getTypeSize(Item *it);

int getStructSize(Item *it);

//获取变量数组内元素个数，例如a[5][3]有3*5=15个
int getArrayNum(Item *it);

//临时保存类型和读取类型
void saveTempType(Item *tp);
Item *getTempType();

//生成临时变量名
char *getTempVar();

//万一是数组。。。妈蛋烦死了简直
void printExp(Item **exp);

//初始化，read和write
void initTable();


// 这个函数生成一条中间代码的空间，同时保证行号唯一
Midcode *newMidcode();

// 这个函数生成一个list项
codeItem *newcodeItem();

// 设置M标记, m标记为LABEL语句后面的标记名
void setM(char *m);

// 获取M标记
char *getM();

// 这个函数merge两个列表到一个列表里
codeItem *mergeList(codeItem *list1, codeItem *list2);

// 这个函数merge一个列表和一条新待填充的语句
codeItem *mergeNode(codeItem *list, Midcode *st);

// 这个函数做字符串替换
void replaceLabel(char *origin, char *label);

// 这个函数回填一个列表,链表中需要回填的地方都用标记为符号 @ ，所以回填的时候只需要替换这个单词为label就可以了
void backpatchList(codeItem *list, char *tag);

// 这个函数生成一个标记名字，标记的名字格式 L_0
char *newTagName();

// 打印中间代码到屏幕
void displayMidcode();

// 保存所有中间代码到文件中
void storeMidcode(char *filename);

// 待填充列表
void displaycodeItem(codeItem *list);
void displayItemList(Item *it);

// 保存当前的exp是不是bool表达式，是返回1，不是返回0
int getBoolean();
void setBoolean(int b);
#endif
