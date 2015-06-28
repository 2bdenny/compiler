#include "generator.h"
#include "GrammarTree.h"

void init_block(){
	blocks = NULL;
	block_tail = NULL;
}

void init_ecode(){
	ecodes = NULL;
}

base_block *new_block(){
	if (NULL == blocks){
		base_block *temp = (base_block *)malloc(sizeof(base_block));
		temp->start = NULL;
		temp->count = 0;
		temp->next = NULL;
		blocks = temp;
		block_tail = temp;
		return temp;
	}
	else {
		base_block *temp = (base_block *)malloc(sizeof(base_block));
		temp->start = NULL;
		temp->count = 0;
		temp->next = NULL;
		block_tail->next = temp;
		block_tail = temp;
		return temp;
	}
}

int get_var(char *var){
	if (var[strlen(var)-1] == '\n') var[strlen(var)-1] = 0;
	var_store *trace = vars;
	while (trace != NULL){
		int cmpsize = max(strlen(trace->name), strlen(var));
		if (memcmp(trace->name, var, cmpsize) == 0) return trace->fpoffset;
		trace = trace->next;
	}
	var_store *tmp = new_var();
	tmp->fpoffset = (-4) * stack_size;
	memcpy(tmp->name, var, strlen(var));
	stack_size ++;

	exe_code *tcode = new_ecode(NULL);
	sprintf(tcode->sentence, "addi $sp, $sp, -4\n");

	return tmp->fpoffset;
}
int get_array(char *var, int size){
	// tail not normal
	if (var[strlen(var)-1] == '\n') var[strlen(var)-1] = 0;

	if (is_var_exist(var)){
		var_store *trace = vars;
		while (trace != NULL){
			int cmpsize = max(strlen(trace->name), strlen(var));
			if (memcmp(trace->name, var, cmpsize) == 0) return trace->fpoffset;
			trace = trace->next;
		}
		return 1;
	}
	else {
		exe_code *tcode = new_ecode(NULL);
		sprintf(tcode->sentence, "\taddi $sp, $sp, %d\n", (-4)*size);
		var_store *tmp = new_var();
		tmp->fpoffset = (-4) * stack_size;
		stack_size += size;
		memcpy(tmp->name, var, strlen(var));
		return tmp->fpoffset;
	}
	return 1;
}

void push_args(){
	var_store *trace = paras;
	while (trace != NULL){
		char *regt = find_reg();
		exe_code *tmp = new_ecode(NULL);
		sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt, trace->fpoffset);
		tmp = new_ecode(NULL);
		sprintf(tmp->sentence, "\tsw %s, 0($sp)\n", regt);
		tmp = new_ecode(NULL);
		sprintf(tmp->sentence, "\taddi $sp, $sp, -4\n");

		stack_size ++;
		trace = trace->next;
	}
//	paras = NULL;
//	para_tail = NULL;
}
void pop_args(){
	var_store *trace = paras;
	while (trace != NULL){
//		char *regt = find_reg();
		exe_code *tmp = new_ecode(NULL);
		sprintf(tmp->sentence, "\taddi $sp, $sp, 4\n");
		tmp = new_ecode(NULL);
//		sprintf(tmp->sentence, "\tsw %s, 0($sp)\n", regt);
//		tmp = new_ecode(NULL);
//		sprintf(tmp->sentence, "\taddi $sp, $sp, -4\n");

		stack_size --;
		trace = trace->next;
	}
	paras = NULL;
	para_tail = NULL;
}
bool isLABEL(Midcode *code){
	if (memcmp(code->sentence, "LABEL", 5) == 0) return true;
	else return false;
}
bool isGOTO(Midcode *code){
	if (memcmp(code->sentence, "GOTO", 4) == 0) return true;
	else return false;
}
bool isIF(Midcode *code){
	if (memcmp(code->sentence, "IF", 2) == 0) return true;
	else return false;
}
bool isRETURN(Midcode *code){
	if (memcmp(code->sentence, "RETURN", 6) == 0) return true;
	else return false;
}
bool isFUNCTION(Midcode *code){
	if (memcmp(code->sentence, "FUNCTION", 8) == 0) return true;
	else return false;
}
bool isEntry(Midcode *code){
	if (isLABEL(code)) return true;
	if (isFUNCTION(code)) return true;
	return false;
}
bool isJump(Midcode *code){
	if (isGOTO(code)) return true;
	if (isIF(code)) return true;
	if (isRETURN(code)) return true;
	if (isCALL(code)) return true;
	return false;
}

// 返回下一个基本块首指令
Midcode *fill_block(Midcode *start, base_block *block){
	if (start == NULL) return NULL;
	block->start = start;
	block->count = 0;
	Midcode *trace = start;

	if (isEntry(start)){
		block->count = 1;
		trace = trace->next;
	}

	while (trace != NULL){
		if (isJump(trace)) {
			block->count++;
			return trace->next;
		}
		if (isEntry(trace)){
			return trace;
		}
		trace = trace->next;
		block->count++;
	}
	return trace;
}

void display_base_block(){
	base_block *trace = blocks;
	while (trace != NULL){
		printf("----------base-block---------\n");
		Midcode *start = trace->start;
		int count = 0;
		while (count < trace->count && start != NULL){
			printf("%s", start->sentence);
			start = start->next;
			count ++;
		}
		trace = trace->next;
	}
}

void part_base_block(){
	Midcode *start = codes;
	while (start != NULL){
//		printf("start is: %s", start->sentence);
		base_block *block = new_block();
		start = fill_block(start, block);
	}
}


void init_regs(){
//	memset(regs, 0, 32*sizeof(reg_store));
	int i;
	for (i = 0; i < 20; i++){
		regs[i].is_used = false;	//全部未使用
		memset(regs[i].name, 0, ID_MAX_LEN);
	}
// 只用20个寄存器
	memcpy(regs[0].name, "$v1", 3);

	memcpy(regs[1].name, "$t0", 3);
	memcpy(regs[2].name, "$t1", 3);
	memcpy(regs[3].name, "$t2", 3);
	memcpy(regs[4].name, "$t3", 3);
	memcpy(regs[5].name, "$t4", 3);
	memcpy(regs[6].name, "$t5", 3);
	memcpy(regs[7].name, "$t6", 3);
	memcpy(regs[8].name, "$t7", 3);

	memcpy(regs[9].name, "$s0", 3);
	memcpy(regs[10].name, "$s1", 3);
	memcpy(regs[11].name, "$s2", 3);
	memcpy(regs[12].name, "$s3", 3);
	memcpy(regs[13].name, "$s4", 3);
	memcpy(regs[14].name, "$s5", 3);
	memcpy(regs[15].name, "$s6", 3);
	memcpy(regs[16].name, "$s7", 3);
	memcpy(regs[17].name, "$s8", 3);

	memcpy(regs[18].name, "$t8", 3);
	memcpy(regs[19].name, "$t9", 3);
}
void init_list(){
	para_offset = 4;
	stack_size = 0;
	vars = NULL;	//变量列表
	paras = NULL;	//参数列表
	para_tail = NULL;
}
// insert from tail
void add_arg(char *var){
//	printf("add arg %s\n", var);
	if (paras == NULL){
		if (var[0] == '*'){	// arg *x
			int offset1 = get_var(var+1);
			var_store *tmp = (var_store *)malloc(sizeof(var_store));

			tmp->fpoffset = offset1;
			memcpy(tmp->name, var, strlen(var));
			tmp->next = paras;

			paras = tmp;
			para_tail = tmp;
		}
		else if (var[0] == '&'){//arg &x
			int offset1 = get_var(var+1);
			var_store *tmp = (var_store *)malloc(sizeof(var_store));
			tmp->fpoffset = offset1;
			memcpy(tmp->name, var, strlen(var));
			tmp->next = paras;

			paras = tmp;
			para_tail = tmp;
		}
		else if (var[0] == '#'){//arg #x
			int offset1 = 1;
			var_store *tmp = (var_store *)malloc(sizeof(var_store));
			tmp->fpoffset = offset1;
			memcpy(tmp->name, var, strlen(var));
			tmp->next = paras;

			paras = tmp;
			para_tail = tmp;
		}
		else {//arg x
			int offset1 = get_var(var);
			var_store *tmp = (var_store *)malloc(sizeof(var_store));
			tmp->fpoffset = offset1;
			memcpy(tmp->name, var, strlen(var));
			tmp->next = paras;

			paras = tmp;
			para_tail = tmp;
		}
	}
	else {
		if (var[0] == '*'){	// arg *x
			int offset1 = get_var(var+1);
			var_store *tmp = (var_store *)malloc(sizeof(var_store));

			tmp->fpoffset = offset1;
			memcpy(tmp->name, var, strlen(var));
			tmp->next = NULL;
			para_tail->next = tmp;
			para_tail = tmp;
		}
		else if (var[0] == '&'){//arg &x
			int offset1 = get_var(var+1);
			var_store *tmp = (var_store *)malloc(sizeof(var_store));
			tmp->fpoffset = offset1;
			memcpy(tmp->name, var, strlen(var));
			tmp->next = NULL;
			para_tail->next = tmp;
			para_tail = tmp;
		}
		else if (var[0] == '#'){//arg #x
			int offset1 = 1;
			var_store *tmp = (var_store *)malloc(sizeof(var_store));
			tmp->fpoffset = offset1;
			memcpy(tmp->name, var, strlen(var));
			tmp->next = NULL;
			para_tail->next = tmp;
			para_tail = tmp;
		}
		else {//arg x
			int offset1 = get_var(var);
			var_store *tmp = (var_store *)malloc(sizeof(var_store));
			tmp->fpoffset = offset1;
			memcpy(tmp->name, var, strlen(var));
			tmp->next = NULL;
			para_tail->next = tmp;
			para_tail = tmp;
		}
	}
}
bool is_var_exist(char *var){
	var_store *trace = vars;
	while (trace != NULL){
		if (memcmp(trace->name, var, max(strlen(var), strlen(trace->name))) == 0) return true;
		trace = trace->next;
	}
	return false;
}
char *get_word(char *sentence, int n){
	char *result = NULL;
	int space_count = 0;
//	int word_count = count_word(sentence);

	int i;
	char *word_start = sentence;
	int word_len = 0;
	for (i = 0; i < min(strlen(sentence), SENTENCE_MAX_LEN); i++){
		if (sentence[i] == ' ') {
			space_count ++;
			if (space_count > n) break;
			else {
				word_start = sentence+i+1;
				continue;
			}
		}
		if (n == space_count) word_len++;
	}
	if (word_len == 0) return NULL;
	else {
		char *word = (char *)malloc(ID_MAX_LEN);
		memset(word, 0, ID_MAX_LEN);
		memcpy(word, word_start, word_len);
//		if (space_count == word_count-1) word[strlen(word)] = 0;
		return word;
	}
}

void generate(){
	init_generate();
	base_block *trace = blocks;
	while (trace != NULL){
		generate_block(trace);
		trace = trace->next;
	}
}
exe_code *new_ecode(base_block *block){
	if (ecodes == NULL){
		exe_code *temp = (exe_code *)malloc(sizeof(exe_code));
		temp->block = block;
		memset(temp->sentence, 0, SENTENCE_MAX_LEN);
		temp->next = NULL;
		ecodes = temp;
		ecode_tail = ecodes;
		return temp;
	}
	else {
		exe_code *temp = (exe_code *)malloc(sizeof(exe_code));
		temp->block = block;
		memset(temp->sentence, 0, SENTENCE_MAX_LEN);
		temp->next = NULL;
		ecode_tail->next = temp;
		ecode_tail = temp;
		return temp;
	}
}
void generate_block(base_block *block){
	int i;
	Midcode *trace = block->start;
	for (i = 0; i < block->count; i++){
		// 初始化所有的寄存器为未使用
		init_regs();
		exe_code *comment = new_ecode(block);
		// 输出一句注释
		sprintf(comment->sentence, "# %s\n", trace->sentence);
		// 基本块结束
		if (trace == NULL){ 
			printf("error: generate code trace=NULL\n");
		}

		// 标签
		if (isLABEL(trace)){	// LABEL X :
			exe_code *tmp = new_ecode(block);
			sprintf(tmp->sentence, "%s:\n", get_word(trace->sentence, 1));
		}
		// 函数
		else if (isFUNCTION(trace)){	// FUNCTION x :
			// 函数名
			char *fname = get_word(trace->sentence, 1);
			exe_code *tmp = new_ecode(block);
			sprintf(tmp->sentence, "%s:\n", get_word(trace->sentence, 1));

			// 如果是main函数，设置一下fp，这是最开始的栈底
			if (memcmp(fname, "main", 4) == 0){
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "move $fp, $sp\n");
			}
			clear_list();
		}
		else if (isASSIGN(trace)){	// :=
			char *var1 = get_word(trace->sentence, 0);
			char *var2 = get_word(trace->sentence, 2);
			if (var1[0] == '*'){
				if (var2[0] == '#'){	// *x = #1
					int offset1 = get_var(var1+1);	//x的偏移
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
				}
				else if (var2[0] == '*'){	// *x = *y
					int offset1 = get_var(var1+1);
					int offset2 = get_var(var2+1);
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
				}
				else if (var2[0] == '&'){	// *x = &y
					int offset1 = get_var(var1+1);
					int offset2 = get_var(var2+1);
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\taddi %s, $fp, %d\n", regt2, offset2);
					tmp = new_ecode(block);
//					sprintf(tmp->sentence, "\tla %s, %s\n", regt2, regt2);
//					tmp = new_ecode(block)//;
					sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
				}
				else {	// *x = y
					int offset1 = get_var(var1+1);
					int offset2 = get_var(var2);
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
				}
			}
			else{
				if (var2[0] == '#'){	// x = #1
					int offset1 = get_var(var1);
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
				}
				else if (var2[0] == '*'){	// x = *y
					int offset1 = get_var(var1);
					int offset2 = get_var(var2+1);
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
				}
				else if (var2[0] == '&'){	// x = &y
					int offset1 = get_var(var1);
					int offset2 = get_var(var2+1);
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\taddi %s, $fp, %d\n", regt2, offset2);
					tmp = new_ecode(block);
//					sprintf(tmp->sentence, "\tla %s, %s\n", regt2, regt2);
//					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
				}
				else {	// x = y
					int offset1 = get_var(var1);
					int offset2 = get_var(var2);
					char *regt1 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset2);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt1, offset1);
				}
			}
		}
		else if (isPLUS(trace)){
			char *var1 = get_word(trace->sentence, 0);
			char *var2 = get_word(trace->sentence, 2);
			char *var3 = get_word(trace->sentence, 4);
			if (var1[0] == '*'){
				if (var2[0] == '#'){
					if (var3[0] == '#'){	// *x = #1 + #2
						int offset1 = get_var(var1+1);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tadd %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
					else {	// *x = #1 + y
						int offset1 = get_var(var1+1);
						int offset3 = get_var(var3);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\taddi %s, %s, %s\n", regt2, regt3, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %d($fp)\n", regt1, offset1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
				}
				else {
					if (var3[0] == '#'){	// *x = y + #1
						int offset1 = get_var(var1+1);
						int offset2 = get_var(var2);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\taddi %s, %s, %s\n", regt2, regt2, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %d($fp)\n", regt1, offset1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
					else {	// *x = y + z
						int offset1 = get_var(var1+1);
						int offset2 = get_var(var2);
						int offset3 = get_var(var3);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tadd %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %d($fp)\n", regt1, offset1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
				}
			}
			else {
				if (var2[0] == '*'){
					if (var3[0] == '*'){	// x = *y + *z
						int offset1 = get_var(var1);
						int offset2 = get_var(var2+1);
						int offset3 = get_var(var3+1);
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tadd %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else if (var3[0] == '#'){	// x = *y + #2
						int offset1 = get_var(var1);
						int offset2 = get_var(var2+1);
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\taddi %s, %s, %s\n", regt2, regt2, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else {	// x = *y + z
						int offset1 = get_var(var1);
						int offset2 = get_var(var2+1);
						int offset3 = get_var(var3);
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tadd %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
				}
				else if (var2[0] == '#'){
					if (var3[0] == '*'){	// x = #2 + *y
						int offset1 = get_var(var1);
						int offset3 = get_var(var3+1);
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\taddi %s, %s, %s\n", regt3, regt3, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt3, offset1);
					}
					else if (var3[0] == '#'){	// x = #1 + #2
						int offset1 = get_var(var1);
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\taddi %s, %s, %s\n", regt3, regt3, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt3, offset1);
					}
					else {	// x = #1 + y
						int offset1 = get_var(var1);
						int offset3 = get_var(var3);
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\taddi %s, %s, %s\n", regt3, regt3, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt3, offset1);
					}
				}
				else {
					if (var3[0] == '*'){	// x = y + *z
						int offset1 = get_var(var1);
						int offset2 = get_var(var2);
						int offset3 = get_var(var3+1);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tadd %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else if (var3[0] == '#'){	// x = y + #2
						int offset1 = get_var(var1);
						int offset2 = get_var(var2);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\taddi %s, %s, %s\n", regt2, regt2, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else {	// x = y + z
						int offset1 = get_var(var1);
						int offset2 = get_var(var2);
						int offset3 = get_var(var3);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tadd %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
				}
			}
		}
		else if (isMINUS(trace)){
			char *var1 = get_word(trace->sentence, 0);
			char *var2 = get_word(trace->sentence, 2);
			char *var3 = get_word(trace->sentence, 4);
			if (var1[0] == '*'){
				if (var2[0] == '#'){
					if (var3[0] == '#'){	// *x = #1 - #2
						int offset1 = get_var(var1+1);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
					else {	// *x = #1 - y
						int offset1 = get_var(var1+1);
						int offset3 = get_var(var3);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
				}
				else {
					int offset1 = get_var(var1+1);
					int offset2 = get_var(var2);
					if (var3[0] == '#'){	// *x = y - #1
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
					else {	// *x = y - z
						int offset3 = get_var(var3);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
				}
			}
			else {
				if (var2[0] == '*'){
					int offset1 = get_var(var1);
					int offset2 = get_var(var2+1);
					if (var3[0] == '*'){	// x = *y - *z
						int offset3 = get_var(var3+1);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else if (var3[0] == '#'){	// x = *y - #2
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else {	// x = *y - z
						int offset3 = get_var(var3);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
				}
				else if (var2[0] == '#'){
					int offset1 = get_var(var1);
					if (var3[0] == '*'){	// x = #2 - *y
						int offset3 = get_var(var3+1);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else if (var3[0] == '#'){	// x = #1 - #2
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else {	// x = #1 - y
						int offset3 = get_var(var3);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
				}
				else {
					int offset1 = get_var(var1);
					int offset2 = get_var(var2);
					if (var3[0] == '*'){	// x = y - *z
						int offset3 = get_var(var3+1);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else if (var3[0] == '#'){	// x = y - #2
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else {	// x = y - z
						int offset3 = get_var(var3);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
				}
			}
		}
		else if (isSTAR(trace)){
			char *var1 = get_word(trace->sentence, 0);
			char *var2 = get_word(trace->sentence, 2);
			char *var3 = get_word(trace->sentence, 4);
			if (var1[0] == '*'){
				int offset1 = get_var(var1+1);
				if (var2[0] == '#'){
					if (var3[0] == '#'){	// *x = #1 * #2
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
					else {	// *x = #1 * z
						int offset3 = get_var(var3);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
				}
				else {
					int offset2 = get_var(var2);
					if (var3[0] == '#'){	// *x = y * #1
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
					else {	// *x = y * z
						int offset3 = get_var(var3);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
				}
			}
			else {
				int offset1 = get_var(var1);
				if (var2[0] == '*'){
					int offset2 = get_var(var2+1);
					if (var3[0] == '*'){	// x = *y * *z
						int offset3 = get_var(var3+1);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else if (var3[0] == '#'){	// x = *y * #2
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else {	// x = *y - z
						int offset3 = get_var(var3);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
				}
				else if (var2[0] == '#'){
					if (var3[0] == '*'){	// x = #2 * *y
						int offset3 = get_var(var3+1);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else if (var3[0] == '#'){	// x = #1 - #2
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else {	// x = #1 * y
						int offset3 = get_var(var3);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
				}
				else {
					int offset2 = get_var(var2);
					if (var3[0] == '*'){	// x = y * *z
						int offset3 = get_var(var3+1);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else if (var3[0] == '#'){	// x = y * #2
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else {	// x = y - z
						int offset3 = get_var(var3);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
				}
			}
		}
		else if (isDIV(trace)){
			char *var1 = get_word(trace->sentence, 0);
			char *var2 = get_word(trace->sentence, 2);
			char *var3 = get_word(trace->sentence, 4);
			if (var1[0] == '*'){
				int offset1 = get_var(var1+1);
				if (var2[0] == '#'){
					if (var3[0] == '#'){	// *x = #1 - #2
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
					else {	// *x = #1 - y
						int offset3 = get_var(var3);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
				}
				else {
					int offset2 = get_var(var2);
					if (var3[0] == '#'){	// *x = y - #1
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
					else {	// *x = y / z
						int offset3 = get_var(var3);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
				}
			}
			else {
				int offset1 = get_var(var1);
				if (var2[0] == '*'){
					int offset2 = get_var(var2+1);
					if (var3[0] == '*'){	// x = *y / *z
						int offset3 = get_var(var3+1);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else if (var3[0] == '#'){	// x = *y - #2
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else {	// x = *y - z
						int offset3 = get_var(var3);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
				}
				else if (var2[0] == '#'){
					if (var3[0] == '*'){	// x = #2 / *y
						int offset3 = get_var(var3+1);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else if (var3[0] == '#'){	// x = #1 - #2
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else {	// x = #1 - y
						int offset3 = get_var(var3);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
				}
				else {
					int offset2 = get_var(var2);
					if (var3[0] == '*'){	// x = y - *z
						int offset3 = get_var(var3+1);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else if (var3[0] == '#'){	// x = y - #2
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
					else {	// x = y - z
						int offset3 = get_var(var3);
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt3, offset3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt2, offset1);
					}
				}
			}
		}
		else if (isGOTO(trace)){
			exe_code *tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tj %s\n", get_word(trace->sentence, 1));
		}
		else if (isIF(trace)){
			char *var1 = get_word(trace->sentence, 1);
			char *var2 = get_word(trace->sentence, 3);
			char *op = get_word(trace->sentence, 2);
			char *label = get_word(trace->sentence, 5);
			if (var1[0] == '*'){
				int offset1 = get_var(var1+1);
				if (var2[0] == '*'){ //*x == *y
					int offset2 = get_var(var2+1);
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt1, regt1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
					tmp = new_ecode(block);
					if (memcmp(op, "==", 2) == 0) sprintf(tmp->sentence, "\tbeq %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "!=", 2) == 0) sprintf(tmp->sentence, "\tbne %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, ">", 1) == 0) sprintf(tmp->sentence, "\tbgt %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "<", 1) == 0) sprintf(tmp->sentence, "\tblt %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, ">=", 2) == 0) sprintf(tmp->sentence, "\tbge %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "<=", 2) == 0) sprintf(tmp->sentence, "\tble %s, %s, %s\n", regt1, regt2, label);
					else sprintf(tmp->sentence, "\t# when if, error occurred\n");
				}
				else if (var2[0] == '#'){	// *x == #y
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt1, regt1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
					tmp = new_ecode(block);
					if (memcmp(op, "==", 2) == 0) sprintf(tmp->sentence, "\tbeq %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "!=", 2) == 0) sprintf(tmp->sentence, "\tbne %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, ">", 1) == 0) sprintf(tmp->sentence, "\tbgt %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "<", 1) == 0) sprintf(tmp->sentence, "\tblt %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, ">=", 2) == 0) sprintf(tmp->sentence, "\tbge %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "<=", 2) == 0) sprintf(tmp->sentence, "\tble %s, %s, %s\n", regt1, regt2, label);
					else sprintf(tmp->sentence, "\t# when if, error occurred\n");
				}
				else {	// *x == y
					int offset2 = get_var(var2);
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt1, regt1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
					tmp = new_ecode(block);
					if (memcmp(op, "==", 2) == 0) sprintf(tmp->sentence, "\tbeq %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "!=", 2) == 0) sprintf(tmp->sentence, "\tbne %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, ">", 1) == 0) sprintf(tmp->sentence, "\tbgt %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "<", 1) == 0) sprintf(tmp->sentence, "\tblt %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, ">=", 2) == 0) sprintf(tmp->sentence, "\tbge %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "<=", 2) == 0) sprintf(tmp->sentence, "\tble %s, %s, %s\n", regt1, regt2, label);
					else sprintf(tmp->sentence, "\t# when if, error occurred\n");
				}
			}
			else if (var1[0] == '#'){
				if (var2[0] == '*'){	// #1 == *y
					int offset2 = get_var(var2+1);
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tli %s, %s\n", regt1, var1+1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
					tmp = new_ecode(block);
					if (memcmp(op, "==", 2) == 0) sprintf(tmp->sentence, "\tbeq %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "!=", 2) == 0) sprintf(tmp->sentence, "\tbne %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, ">", 1) == 0) sprintf(tmp->sentence, "\tbgt %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "<", 1) == 0) sprintf(tmp->sentence, "\tblt %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, ">=", 2) == 0) sprintf(tmp->sentence, "\tbge %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "<=", 2) == 0) sprintf(tmp->sentence, "\tble %s, %s, %s\n", regt1, regt2, label);
					else sprintf(tmp->sentence, "\t# when if, error occurred\n");
				}
				else if (var2[0] == '#'){	// #1 == #1
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tli %s, %s\n", regt1, var1+1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
					tmp = new_ecode(block);
					if (memcmp(op, "==", 2) == 0) sprintf(tmp->sentence, "\tbeq %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "!=", 2) == 0) sprintf(tmp->sentence, "\tbne %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, ">", 1) == 0) sprintf(tmp->sentence, "\tbgt %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "<", 1) == 0) sprintf(tmp->sentence, "\tblt %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, ">=", 2) == 0) sprintf(tmp->sentence, "\tbge %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "<=", 2) == 0) sprintf(tmp->sentence, "\tble %s, %s, %s\n", regt1, regt2, label);
					else sprintf(tmp->sentence, "\t# when if, error occurred\n");
				}
				else {	// #1 == y
					int offset2 = get_var(var2);
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tli %s, %s\n", regt1, var1+1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
					tmp = new_ecode(block);
					if (memcmp(op, "==", 2) == 0) sprintf(tmp->sentence, "\tbeq %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "!=", 2) == 0) sprintf(tmp->sentence, "\tbne %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, ">", 1) == 0) sprintf(tmp->sentence, "\tbgt %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "<", 1) == 0) sprintf(tmp->sentence, "\tblt %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, ">=", 2) == 0) sprintf(tmp->sentence, "\tbge %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "<=", 2) == 0) sprintf(tmp->sentence, "\tble %s, %s, %s\n", regt1, regt2, label);
					else sprintf(tmp->sentence, "\t# when if, error occurred\n");
				}
			}
			else{
				int offset1 = get_var(var1);
				if (var2[0] == '*'){	// x == *y
					int offset2 = get_var(var2+1);
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
					tmp = new_ecode(block);
					if (memcmp(op, "==", 2) == 0) sprintf(tmp->sentence, "\tbeq %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "!=", 2) == 0) sprintf(tmp->sentence, "\tbne %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, ">", 1) == 0) sprintf(tmp->sentence, "\tbgt %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "<", 1) == 0) sprintf(tmp->sentence, "\tblt %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, ">=", 2) == 0) sprintf(tmp->sentence, "\tbge %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "<=", 2) == 0) sprintf(tmp->sentence, "\tble %s, %s, %s\n", regt1, regt2, label);
					else sprintf(tmp->sentence, "\t# when if, error occurred\n");
				}
				else if (var2[0] == '#'){	//x == #1
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
					tmp = new_ecode(block);
					if (memcmp(op, "==", 2) == 0) sprintf(tmp->sentence, "\tbeq %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "!=", 2) == 0) sprintf(tmp->sentence, "\tbne %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, ">", 1) == 0) sprintf(tmp->sentence, "\tbgt %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "<", 1) == 0) sprintf(tmp->sentence, "\tblt %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, ">=", 2) == 0) sprintf(tmp->sentence, "\tbge %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "<=", 2) == 0) sprintf(tmp->sentence, "\tble %s, %s, %s\n", regt1, regt2, label);
					else sprintf(tmp->sentence, "\t# when if, error occurred\n");
				}
				else {	// x == y
					int offset2 = get_var(var2);
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt2, offset2);
					tmp = new_ecode(block);
					if (memcmp(op, "==", 2) == 0) sprintf(tmp->sentence, "\tbeq %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "!=", 2) == 0) sprintf(tmp->sentence, "\tbne %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, ">", 1) == 0) sprintf(tmp->sentence, "\tbgt %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "<", 1) == 0) sprintf(tmp->sentence, "\tblt %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, ">=", 2) == 0) sprintf(tmp->sentence, "\tbge %s, %s, %s\n", regt1, regt2, label);
					else if (memcmp(op, "<=", 2) == 0) sprintf(tmp->sentence, "\tble %s, %s, %s\n", regt1, regt2, label);
					else sprintf(tmp->sentence, "\t# when if, error occurred\n");
				}
			}
		}
		else if (isRETURN(trace)){
			char *var1 = get_word(trace->sentence, 1);
			if (var1[0] == '*'){ // return *x
				int offset1 = get_var(var1+1);
				char *regt1 = find_reg();
				exe_code *tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt1, regt1);
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tmove $v0, %s\n", regt1);

				// pop var of this function & clear vars & paras & reset regs
//				clear_list();
				if (stack_size > 0) {
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "addi $sp, $sp, %d\n", 4*stack_size);
				}

				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tjr $ra\n");
			}
			else if (var1[0] == '&'){ // return &x
				int offset1 = get_var(var1+1);
				exe_code *tmp = new_ecode(block);
				sprintf(tmp->sentence, "\taddi $v0, $fp, %d\n", offset1);

//				clear_list();
				if (stack_size > 0) {
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "addi $sp, $sp, %d\n", 4*stack_size);
				}

				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tjr $ra\n");
			}
			else if (var1[0] == '#'){// return #x
				exe_code *tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tli $v0, %s\n", var1+1);
//				clear_list();
				if (stack_size > 0) {
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "addi $sp, $sp, %d\n", 4*stack_size);
				}
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tjr $ra\n");
			}
			else {// return x
				int offset1 = get_var(var1);
				exe_code *tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tlw $v0, %d($fp)\n", offset1);
//				clear_list();
				if (stack_size > 0) {
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "addi $sp, $sp, %d\n", 4*stack_size);
				}
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tjr $ra\n");
			}
		}
		else if (isDEC(trace)){
			char var1[ID_MAX_LEN];
			int size;
			memset(var1, 0, ID_MAX_LEN);
			sscanf(trace->sentence, "DEC %s %d", var1, &size);

			get_array(var1, size);
		}
		else if (isARG(trace)){
			char *var1 = get_word(trace->sentence, 1);
			if (var1[0] == '*'){	// arg *x
				int offset1 = get_var(var1+1);
				add_arg(var1);
			}
			else if (var1[0] == '&'){//arg &x
				int offset1 = get_var(var1+1);
				add_arg(var1);
			}
			else if (var1[0] == '#'){//arg #x
				add_arg(var1);
			}
			else {//arg x
				int offset1 = get_var(var1);
				add_arg(var1);
			}
		}
		else if (isCALL(trace)){
			char *label = get_word(trace->sentence, 3);
			char *var1 = get_word(trace->sentence, 0);

			exe_code *tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tsw $fp, 0($sp)\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\taddi $sp, $sp, -4\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tsw $ra, 0($sp)\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\taddi $sp, $sp, -4\n");

			// push all args to stack, clear args list
			push_args();

			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tmove $fp, $sp\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tjal %s\n", label);
			tmp = new_ecode(block);

			pop_args();

			sprintf(tmp->sentence, "\taddi $sp, $sp, 4\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tlw $ra, 0($sp)\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\taddi $sp, $sp, 4\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tlw $fp, 0($sp)\n");

			if (var1[0] == '*'){//*x = call f
				int offset1 = get_var(var1);
				char *regt1 = find_reg();
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tsw $v0, 0(%s)\n", regt1);
			}
			else {//x=call f
				int offset1 = get_var(var1);
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tsw $v0, %d($fp)\n", offset1);
			}
		}
		else if (isPARAM(trace)){
			char *var1 = get_word(trace->sentence, 1);
			int offset1 = get_var(var1);

			char *regt = find_reg();
			exe_code *tmp = new_ecode(block);
//			sprintf(tmp->sentence, "\taddi $sp, $sp, 4\n");
//			tmp = new_ecode(block);
//			sprintf(tmp->sentence, "\tmove $fp, $sp\n");
//			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt, para_offset);
			para_offset += 4;
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tsw %s, %d($fp)\n", regt, offset1);
		}
		else if (isREAD(trace)){
			char *var1 = get_word(trace->sentence, 1);
	
			exe_code *tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tsw $fp, 0($sp)\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\taddi $sp, $sp, -4\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tsw $ra, 0($sp)\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\taddi $sp, $sp, -4\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tmove $fp, $sp\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tjal read\n");
			tmp = new_ecode(block);

//			pop_args();
			paras = NULL;

			sprintf(tmp->sentence, "\taddi $sp, $sp, 4\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tlw $ra, 0($sp)\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\taddi $sp, $sp, 4\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tlw $fp, 0($sp)\n");		
			if (var1[0] == '*'){ //read *x
				int offset1 = get_var(var1+1);
				char *regt1 = find_reg();
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt1, offset1);
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tsw $v0, 0(%s)\n", regt1);
			}
			else {// read x
				int offset1 = get_var(var1);
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tsw $v0, %d($fp)\n", offset1);
			}
		}
		else if (isWRITE(trace)){// write *x
//			push_args();
			char *var1 = get_word(trace->sentence, 1);

			if (var1[0] == '*'){
				int offset1 = get_var(var1+1);
				char *regt = find_reg();
				exe_code *tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tlw %s, %d($fp)\n", regt, offset1);
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tlw $a0, 0(%s)\n", regt);
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tsw $fp, 0($sp)\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\taddi $sp, $sp, -4\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tsw $ra, 0($sp)\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\taddi $sp, $sp, -4\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tmove $fp, $sp\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tjal write\n");
				tmp = new_ecode(block);
			//	pop_args();
				paras = NULL;

				sprintf(tmp->sentence, "\taddi $sp, $sp, 4\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tlw $ra, 0($sp)\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\taddi $sp, $sp, 4\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tlw $fp, 0($sp)\n");	
			}
			else if(var1[0] == '#'){// write #x
				char *regt = find_reg();
				exe_code *tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tli $a0, %s\n", var1+1);
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tsw $fp, 0($sp)\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\taddi $sp, $sp, -4\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tsw $ra, 0($sp)\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\taddi $sp, $sp, -4\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tmove $fp, $sp\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tjal write\n");
//			pop_args();
				paras = NULL;
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\taddi $sp, $sp, 4\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tlw $ra, 0($sp)\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\taddi $sp, $sp, 4\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tlw $fp, 0($sp)\n");	
			}
			else if(var1[0] == '&') {//write &x
				char *regt = find_reg();
				int offset1 = get_var(var1+1);
				exe_code *tmp = new_ecode(block);
				sprintf(tmp->sentence, "\taddi $a0, $fp, %d\n", offset1);
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tsw $fp, 0($sp)\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\taddi $sp, $sp, -4\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tsw $ra, 0($sp)\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\taddi $sp, $sp, -4\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tmove $fp, $sp\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tjal write\n");
//			pop_args();
				paras = NULL;
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\taddi $sp, $sp, 4\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tlw $ra, 0($sp)\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\taddi $sp, $sp, 4\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tlw $fp, 0($sp)\n");	
			}
			else {//write x
				int offset1 = get_var(var1);
				char *regt = find_reg();
				exe_code *tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tlw $a0, %d($fp)\n", offset1);
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tsw $fp, 0($sp)\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\taddi $sp, $sp, -4\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tsw $ra, 0($sp)\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\taddi $sp, $sp, -4\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tmove $fp, $sp\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tjal write\n");
//			pop_args();
				paras = NULL;
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\taddi $sp, $sp, 4\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tlw $ra, 0($sp)\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\taddi $sp, $sp, 4\n");
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tlw $fp, 0($sp)\n");	
			}
		}
		else {
			printf("can't classify: %s\n", trace->sentence);
		}
		trace = trace->next;
	}
}

int count_word(char *sentence){
	int i;
	for (i = 0; i < MAX_WORDS; i++){
		if (NULL == get_word(sentence, i)) return i;
	}
}
bool isASSIGN(Midcode *code){
	int num = count_word(code->sentence);
	if (num == 3){
		char *assignop = get_word(code->sentence, 1);
		if (memcmp(assignop, ":=", 2) == 0) return true;
	}
	return false;
}
bool isPLUS(Midcode *code){
	int num = count_word(code->sentence);
	if (num == 5) {
		char *assignop = get_word(code->sentence, 1);
		char *plusop = get_word(code->sentence, 3);
		if (memcmp(assignop, ":=", 2) == 0 && memcmp(plusop, "+", 1) == 0) return true;
	}
	return false;
}
bool isMINUS(Midcode *code){
	int num = count_word(code->sentence);
	if (num == 5) {
		char *assignop = get_word(code->sentence, 1);
		char *plusop = get_word(code->sentence, 3);
		if (memcmp(assignop, ":=", 2) == 0 && memcmp(plusop, "-", 1) == 0) return true;
	}
	return false;
}
bool isSTAR(Midcode *code){
	int num = count_word(code->sentence);
	if (num == 5) {
		char *assignop = get_word(code->sentence, 1);
		char *plusop = get_word(code->sentence, 3);
		if (memcmp(assignop, ":=", 2) == 0 && memcmp(plusop, "*", 1) == 0) return true;
	}
	return false;
}
bool isDIV(Midcode *code){
	int num = count_word(code->sentence);
	if (num == 5) {
		char *assignop = get_word(code->sentence, 1);
		char *plusop = get_word(code->sentence, 3);
		if (memcmp(assignop, ":=", 2) == 0 && memcmp(plusop, "/", 1) == 0) return true;
	}
	return false;
}
bool isDEC(Midcode *code){
	int num = count_word(code->sentence);
	if (num == 3) {
		char *dec = get_word(code->sentence, 0);
		if (memcmp(dec, "DEC", 3) == 0) return true;
	}
	return false;
}
bool isARG(Midcode *code){
	int num = count_word(code->sentence);
	if (num == 2) {
		char *arg = get_word(code->sentence, 0);
		if (memcmp(arg, "ARG", 3) == 0) return true;
	}
	return false;
}
bool isCALL(Midcode *code){
	int num = count_word(code->sentence);
	if (num == 4) {
		char *call = get_word(code->sentence, 2);
		if (memcmp(call, "CALL", 4) == 0) return true;
	}
	return false;
}
bool isPARAM(Midcode *code){
	int num = count_word(code->sentence);
	if (num == 2) {
		char *param = get_word(code->sentence, 0);
		if (memcmp(param, "PARAM", 5) == 0) return true;
	}
	return false;
}
bool isREAD(Midcode *code){
	int num = count_word(code->sentence);
	if (num == 2){
		char *read = get_word(code->sentence, 0);
		if (memcmp(read, "READ", 4) == 0) return true;
	}
	return false;
}
bool isWRITE(Midcode *code){
	int num = count_word(code->sentence);
	if (num == 2) {
		char *write = get_word(code->sentence, 0);
		if (memcmp(write, "WRITE", 5) == 0) return true;
	}
	return false;
}

void store_exe_code(char *filename){
	efile = fopen(filename, "w+");
	if (NULL == efile) {
		printf("Error occurred in open asm file\n");
		return ;
	}
	exe_code *trace = ecodes;
	while (NULL != trace){
		fprintf(efile, "%s", trace->sentence);
		trace = trace->next;
	}
	fclose(efile);
}
void init_generate(){
	// 初始化不会变的代码
	exe_code *tmp = new_ecode(NULL);
	sprintf(tmp->sentence, ".data\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "_prompt: .asciiz \"Enter an integer:\"\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "_ret: .asciiz \"\\n\"\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, ".globl main\n");

//	init_all_var();

	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, ".text\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "read:\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "\tli $v0, 4\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "\tla $a0, _prompt\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "\tsyscall\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "\tli $v0, 5\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "\tsyscall\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "\tjr $ra\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "write:\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "\tli $v0, 1\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "\tsyscall\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "\tli $v0, 4\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "\tla $a0, _ret\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "\tsyscall\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "\tmove $v0, $0\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "\tjr $ra\n");

	// 初始化所有寄存器
	init_regs();
	// 初始化变量列表和参数列表
	init_list();
}

char *find_reg(){
	char *result = (char *)malloc(ID_MAX_LEN);
	memset(result, 0, ID_MAX_LEN);
	int i;
	for (i = 0; i < 20; i ++){
		if (regs[i].is_used == false){
			memcpy(result, regs[i].name, strlen(regs[i].name));
//			regs[i].list = (var_list_node *)malloc(sizeof(var_list_node));
			regs[i].is_used = true;
			return result;
		}
	}
	return "regnotfind";
}

var_store *new_var(){
	var_store *tmp = (var_store *)malloc(sizeof(var_store));
	memset(tmp, 0, sizeof(var_store));
	if (vars == NULL){
		vars = tmp;
	}
	else {
		tmp->next = vars;
		vars = tmp;
	}
	return tmp;
}
void clear_list(){
/*	exe_code *tmp = new_ecode(NULL);
	if (stack_size > 0){
		sprintf(tmp->sentence, "\taddi $sp, $sp, %d\n", (4)*stack_size);
		stack_size = 0;
	}
	else */stack_size = 0;
	para_offset = 4;
	vars = NULL;
	paras = NULL;
	para_tail = NULL;
}
