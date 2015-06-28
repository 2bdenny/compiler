#ifndef GENERATOR_H_
#define GENERATOR_H_
#include "GrammarTree.h"
#include "stack.h"

// 假设一句句子最多有29个空格
#define MAX_WORDS 30
// 基本块定义
typedef struct base_block_{
	Midcode *start;
	int count;
	struct base_block_ *next;
} base_block;
// 基本块存储
base_block *blocks;
base_block *block_tail;
//------------------------以上基本块相关----------------------------

// 寄存器存储定义
typedef struct reg_store_{
	char name[ID_MAX_LEN];	// 寄存器名
	bool is_used;
} reg_store;

// 一个变量的存储位置的定义
typedef struct var_store_{
	int fpoffset;
	char name[ID_MAX_LEN];	//变量名
	struct var_store_ *next;
} var_store;

reg_store regs[20];	//存寄存器
int stack_size;		//栈里的变量个数，当get var的时候可能增加
int para_offset;
var_store *vars;	//存变量
var_store *paras;	//存参数
var_store *para_tail;
//------------------------以上选择寄存器相关------------------------

// 机器代码存储
typedef struct exe_code_{
	base_block *block;	// 对应的是哪个基本块
	char sentence[SENTENCE_MAX_LEN];	//代码
	struct exe_code_ *next;	// 下一条代码
} exe_code;
// 保存所有生成的机器代码
exe_code *ecodes;
exe_code *ecode_tail;
// 写入的.s文件
FILE *efile;
//------------------以上机器代码生成部分------------------------

// 初始化基本块
void init_block();

// 判断语句
bool isLABEL(Midcode *code);
bool isGOTO(Midcode *code);
bool isIF(Midcode *code);
bool isRETURN(Midcode *code);
bool isFUNCTION(Midcode *code);
bool isEntry(Midcode *code);
bool isJump(Midcode *code);

// 获得一个新基本块的空间
base_block *new_block();

// 填充一个新的基本块
Midcode *fill_block(Midcode *start, base_block *block);

// 这个函数调用之后生成所有的机器代码
void generate();

// 展示所有的基本块
void display_base_block();

// 切分基本块
void part_base_block();

// 获取sentence的第n个单词，n从0开始，n大于单词总数就返回NULL
char *get_word(char *sentence, int n);

// 计算一个句子有几个单词
int count_word(char *sentence);

// 这个函数为一个基本块生成机器代码
void generate_block(base_block *block);

// 以下判断是哪种类型的指令
// bool isLABEL(); 	//同判断语句
// bool isFUNCTION();	//同判断语句
bool isASSIGN(Midcode *code);	// x := y 的赋值
				/* 包含了
				   	x := y
					x := &y
					x := *y
					*x := y
					*x := *y
				*/

bool isPLUS(Midcode *code);	// x := y + z 的赋值
bool isMINUS(Midcode *code);	// x := y - z 的赋值
bool isSTAR(Midcode *code);	// x := y * z
bool isDIV(Midcode *code);	// x := y / z
// bool isGOTO()	//同判断语句
// bool isIF()		//同判断语句
// bool isRETURN()	//同判断语句
bool isDEC(Midcode *code);	// DEC 声明数组
bool isARG(Midcode *code);	// ARG 传参
bool isCALL(Midcode *code);	// 调用函数
bool isPARAM(Midcode *code);	// 函数参数声明
bool isREAD(Midcode *code);	// read函数调用
bool isWRITE(Midcode *code);	// write函数调用
//---------------以上基本块对应的函数---------------------

// 找到变量var现在保存在哪个寄存器，如果没有就调用find_reg找到一个新的寄存器给var
//char *get_reg(char *var);
// 找到一个空的寄存器，然后返回这个寄存器
// 然后返回这个寄存器
char *find_reg();
// 这个函数获取一个变量在内存里的地址（相对于fp的偏移）,把这个地址放到一个寄存器里，然后返回这个寄存器
//char *get_addr(char *var);
// 这个函数初始化所有的寄存器
void init_regs();
// 这个函数初始化变量列表和参数列表
void init_list();

// 如果变量已经存在，返回相对于$fp的偏移
// 如果不存在，往栈里增加一个变量，增加这个变量到变量里列表里，然后返回这个变量相当于$fp的偏移
int get_var(char *var);
// dec 了一个数组
int get_array(char *var, int size);

// 只是增加var到参数列表里
void add_arg(char *var);

// 从栈顶退出一个变量，这个变量就是当前想要的arg
// 把这个变量赋值给当前设置的arg变量，然后把arg存到变量列表里
// lw $reg, 0($sp)
// addi $sp, $sp, 4
// sw $reg, offset($fp)
void get_arg(char *reg);

// 从vars根据变量名获取offset
int get_offset(char *var);

// 从vars检查变量存储，变量是否存在
bool is_var_exist(char *var);

// 新建一个var
var_store *new_var();

// 清空变量列表和参数列表，初始化参数列表，退栈
void clear_list();
//---------------以上寄存器对应的函数---------------------

// 这个函数把生成的机器指令写入.s文件
void store_exe_code(char *filename);
exe_code *new_ecode(base_block *block);
void init_all_var();
void init_ecode();
void init_generate();
//---------------以上文件写入和展示对应的函数------------------
#endif
