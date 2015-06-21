#include "generator.h"
#include "GrammarTree.h"

int arg_count = 0;
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
		regs[i].list = NULL;
		memset(regs[i].name, 0, ID_MAX_LEN);
	}

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
void init_vars(){
	vars = NULL;
}
void add_var(char *var, int size){
	if (var[0] == '&') var = var+1;
	if (!is_var_exist(var)){
		var_store *tmp = (var_store *)malloc(sizeof(var_store));
		memset(tmp->name, 0, ID_MAX_LEN);
		tmp->list = NULL;
		tmp->next = NULL;
		memcpy(tmp->name, var, strlen(var));
		if (vars == NULL) vars = tmp;
		else {
			tmp->next = vars;
			vars = tmp;
		}
	}
}
void add_arg(char *reg){
//	printf("\t\t\tadd_arg %s\n", reg);
/*	if (arg_count < 4){
		exe_code *tmp = new_ecode(NULL);
		sprintf(tmp->sentence, "\tmove $a%d, %s\n", arg_count, reg);
	}
	else {*/
		exe_code *tmp = new_ecode(NULL);
		sprintf(tmp->sentence, "\tmove $sp, %s\n", reg);
		tmp = new_ecode(NULL);
		sprintf(tmp->sentence, "\taddi $sp, $sp, 4\n");
//	}
//	arg_count ++;
}
void get_arg(char *reg){
//	if (arg_count >= 4){
		exe_code *tmp = new_ecode(NULL);
		sprintf(tmp->sentence, "\tmove %s, $sp\n", reg);
		tmp = new_ecode(NULL);
		sprintf(tmp->sentence, "\taddi $sp, $sp, -4\n");
//	}
/*	else {
		exe_code *tmp = new_ecode(NULL);
		sprintf(tmp->sentence, "\tmove %s, $a%d\n", reg, arg_count);
	}
	arg_count --;*/
}
bool is_var_exist(char *var){
	var_store *trace = vars;
	while (trace != NULL){
		if (memcmp(trace->name, var, min(strlen(var), strlen(trace->name))) == 0) return true;
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
		init_regs();
//		printf("%s\n", trace->sentence);
		if (trace == NULL){ 
			printf("error: generate code trace=NULL\n");
		}

		if (isLABEL(trace)){	// LABEL X :
			exe_code *tmp = new_ecode(block);
			sprintf(tmp->sentence, "%s:\n", get_word(trace->sentence, 1));
		}
		else if (isFUNCTION(trace)){	// FUNCTION x :
			exe_code *tmp = new_ecode(block);
			sprintf(tmp->sentence, "%s:\n", get_word(trace->sentence, 1));
		}
		else if (isASSIGN(trace)){	// :=
			char *var1 = get_word(trace->sentence, 0);
			char *var2 = get_word(trace->sentence, 2);
			if (var1[0] == '*'){
				if (var2[0] == '#'){	// *x = #1
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1+1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
				}
				else if (var2[0] == '*'){	// *x = *y
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2+1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1+1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
				}
				else if (var2[0] == '&'){	// *x = &y
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tla %s, %s\n", regt2, var2+1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1+1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
				}
				else {	// *x = y
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1+1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
				}
			}
			else{
				if (var2[0] == '#'){	// x = #1
					char *reg1 = get_reg(var1);
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
				}
				else if (var2[0] == '*'){	// x = *y
					char *reg1 = get_reg(var1);
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2+1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
				}
				else if (var2[2] == '&'){	// x = &y
					char *addr2 = get_addr(var2+1);
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tla %s, %s\n", regt2, var2+1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
				}
				else {	// x = y
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var2);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tsw %s, %s\n", regt1, var1);
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
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tadd %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
					else {	// *x = #1 + y
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %s\n", regt3, var3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\taddi %s, %s, %s\n", regt2, regt3, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %s\n", regt1, var1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
				}
				else {
					if (var3[0] == '#'){	// *x = y + #1
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %s\n", regt2, var2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\taddi %s, %s, %s\n", regt2, regt2, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %s\n", regt1, var1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
					else {	// *x = y + z
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %s\n", regt2, var2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %s\n", regt3, var3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tadd %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %s\n", regt1, var1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
				}
			}
			else {
				if (var2[0] == '*'){
					if (var3[0] == '*'){	// x = *y + *z
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tadd %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
					else if (var3[0] == '#'){	// x = *y + #2
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\taddi %s, %s, %s\n", regt2, regt2, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
					else {	// x = *y + z
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tadd %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
				}
				else if (var2[0] == '#'){
					if (var3[0] == '*'){	// x = #2 + *y
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\taddi %s, %s, %s\n", regt3, regt3, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt3, var1);
					}
					else if (var3[0] == '#'){	// x = #1 + #2
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\taddi %s, %s, %s\n", regt3, regt3, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt3, var1);
					}
					else {	// x = #1 + y
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\taddi %s, %s, %s\n", regt3, regt3, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt3, var1);
					}
				}
				else {
					if (var3[0] == '*'){	// x = y + *z
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tadd %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
					else if (var3[0] == '#'){	// x = y + #2
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\taddi %s, %s, %s\n", regt2, regt2, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
					else {	// x = y + z
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tadd %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
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
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
					else {	// *x = #1 - y
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
				}
				else {
					if (var3[0] == '#'){	// *x = y - #1
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
					else {	// *x = y - z
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
				}
			}
			else {
				if (var2[0] == '*'){
					if (var3[0] == '*'){	// x = *y - *z
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
					else if (var3[0] == '#'){	// x = *y - #2
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
					else {	// x = *y - z
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
				}
				else if (var2[0] == '#'){
					if (var3[0] == '*'){	// x = #2 - *y
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
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
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
					else {	// x = #1 - y
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
				}
				else {
					if (var3[0] == '*'){	// x = y - *z
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
					else if (var3[0] == '#'){	// x = y - #2
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
					else {	// x = y - z
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsub %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
				}
			}
		}
		else if (isSTAR(trace)){
			char *var1 = get_word(trace->sentence, 0);
			char *var2 = get_word(trace->sentence, 2);
			char *var3 = get_word(trace->sentence, 4);
			if (var1[0] == '*'){
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
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
					else {	// *x = #1 - y
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
				}
				else {
					if (var3[0] == '#'){	// *x = y - #1
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
					else {	// *x = y - z
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
				}
			}
			else {
				if (var2[0] == '*'){
					if (var3[0] == '*'){	// x = *y - *z
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
					else if (var3[0] == '#'){	// x = *y - #2
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
					else {	// x = *y - z
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
				}
				else if (var2[0] == '#'){
					if (var3[0] == '*'){	// x = #2 - *y
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
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
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
					else {	// x = #1 - y
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
				}
				else {
					if (var3[0] == '*'){	// x = y - *z
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
					else if (var3[0] == '#'){	// x = y - #2
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
					else {	// x = y - z
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmul %s, %s, %s\n", regt2, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
				}
			}
		}
		else if (isDIV(trace)){
			char *var1 = get_word(trace->sentence, 0);
			char *var2 = get_word(trace->sentence, 2);
			char *var3 = get_word(trace->sentence, 4);
			if (var1[0] == '*'){
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
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
					else {	// *x = #1 - y
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
				}
				else {
					if (var3[0] == '#'){	// *x = y - #1
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, 0(%s)\n", regt2, regt1);
					}
					else {	// *x = y - z
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1+1);
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
				if (var2[0] == '*'){
					if (var3[0] == '*'){	// x = *y - *z
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
					else if (var3[0] == '#'){	// x = *y - #2
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
					else {	// x = *y - z
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt2, regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
				}
				else if (var2[0] == '#'){
					if (var3[0] == '*'){	// x = #2 - *y
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
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
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
					else {	// x = #1 - y
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
				}
				else {
					if (var3[0] == '*'){	// x = y - *z
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt3, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
					else if (var3[0] == '#'){	// x = y - #2
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tli %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
					}
					else {	// x = y - z
						char *regt1 = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tlw %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tdiv %s, %s\n", regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tmflo %s\n", regt2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "\tsw %s, %s\n", regt2, var1);
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
				if (var2[0] == '*'){ //*x == *y
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1+1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt1, regt1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2+1);
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
					sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1+1);
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
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1+1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt1, regt1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
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
				if (var2[0] == '*'){	// #x == *y
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tli %s, %s\n", regt1, var1+1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2+1);
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
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tli %s, %s\n", regt1, var1+1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
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
				if (var2[0] == '*'){	// x == *y
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2+1);
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
					sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1);
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
					char *regt1 = find_reg();
					char *regt2 = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %s\n", regt1, var1);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "\tlw %s, %s\n", regt2, var2);
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
			if (var1[0] == '*'){
				char *reg1 = get_reg(var1+1);
				char *regt1 = find_reg();
				exe_code *tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt1, reg1);
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tmove $v0, %s\n", regt1);
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tjr $ra\n");
			}
			else if (var1[0] == '&'){
				char *addr1 = get_addr(var1+1);
				exe_code *tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tmove $v0, %s\n", addr1);
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tjr $ra\n");
			}
			else if (var1[0] == '#'){
				exe_code *tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tli $v0, %s\n", var1+1);
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tjr $ra\n");
			}
			else {
				char *reg1 = get_reg(var1);
				exe_code *tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tmove $v0, %s\n", reg1);
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tjr $ra\n");
			}
		}
		else if (isDEC(trace)){
			char var1[ID_MAX_LEN];
			memset(var1, 0, ID_MAX_LEN);
			int size;
			sscanf(trace->sentence, "DEC %s %d\n", var1, &size);
			add_var(var1, size);
		}
		else if (isARG(trace)){
			char *var1 = get_word(trace->sentence, 1);
//			printf("\t\t\t\t arg %s\n", var1);
			if (var1[0] == '*'){
				char *reg1 = get_reg(var1+1);
				char *regt1 = find_reg();
				exe_code *tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tlw %s, 0(%s)\n", regt1, reg1);
				tmp = new_ecode(block);
				add_arg(regt1);
			}
			else if (var1[0] == '&'){
				char *addr1 = get_addr(var1+1);
				add_arg(addr1);
			}
			else if (var1[0] == '#'){
				char *regt1 = find_reg();
				exe_code *tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tli %s, %s\n", regt1, var1+1);
				add_arg(regt1);
			}
			else {
				char *reg1 = get_reg(var1);
				char *regt1 = find_reg();
				exe_code *tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tmove %s, %s\n", regt1, reg1);
				add_arg(regt1);
			}
		}
		else if (isCALL(trace)){
			char *label = get_word(trace->sentence, 3);
			char *var1 = get_word(trace->sentence, 0);
			exe_code *tmp = new_ecode(block);
			sprintf(tmp->sentence, "\taddi $sp, $sp, -4\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tsw $ra, 0($sp)\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tjal %s\n", label);
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\taddi $sp, $sp, 4\n");
			tmp = new_ecode(block);
			if (var1[0] == '*'){
				char *reg1 = get_reg(var1+1);
				sprintf(tmp->sentence, "\tsw $v0, 0(%s)\n", reg1);
			}
			else {
				char *reg1 = get_reg(var1);
				sprintf(tmp->sentence, "\tmove %s, $v0\n", reg1);
			}
		}
		else if (isPARAM(trace)){
			char *var1 = get_word(trace->sentence, 1);
			char *reg1 = get_reg(var1);
			get_arg(reg1);
		}
		else if (isREAD(trace)){
			char *var1 = get_word(trace->sentence, 0);

			exe_code *tmp = new_ecode(block);
			sprintf(tmp->sentence, "\taddi $sp, $sp, -4\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tsw $ra, 0($sp)\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tjal read\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tlw $ra, 0($sp)\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\taddi $sp, $sp, 4\n");
			if (var1[0] == '*'){
				char *reg1 = get_reg(var1+1);
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tsw $v0, 0(%s)\n", reg1);
			}
			else {
				char *reg1 = get_reg(var1);
				tmp = new_ecode(block);
				sprintf(tmp->sentence, "\tmove %s, $v0\n", reg1);
			}
		}
		else if (isWRITE(trace)){
			exe_code *tmp = new_ecode(block);
			sprintf(tmp->sentence, "\taddi $sp, $sp, -4\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tsw $ra, 0($sp)\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tjal write\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\tlw $ra, 0($sp)\n");
			tmp = new_ecode(block);
			sprintf(tmp->sentence, "\taddi $sp, $sp, 4\n");
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
	exe_code *tmp = new_ecode(NULL);
	sprintf(tmp->sentence, ".data\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "_prompt: .asciiz \"Enter an integer:\"\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "_ret: .asciiz \"\\n\"\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, ".globl main\n");

	init_all_var();

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

	init_regs();
	init_vars();
}

char *get_reg(char *var){
	char *result = (char *)malloc(ID_MAX_LEN);
	memset(result, 0, ID_MAX_LEN);
	int i;
	var_store *trace = vars;
	while (trace != NULL){
		if (memcmp(trace->name, var, min(strlen(var), strlen(trace->name))) == 0) break;
		trace = trace->next;
	}
	// var is exist
	if (trace != NULL){
		if (trace->list != NULL){
			memcpy(result, trace->list->addr, strlen(trace->list->addr));
		}
		else {	// var is exist but not in register
			char *reg = find_reg();
			memcpy(result, reg, strlen(reg));
			add_var_reg(trace, reg);
			add_reg_var(reg, var);
		}
	}
	else {// var not exist
		var_store *tmp = new_var();	// new var
		memcpy(tmp->name, var, strlen(var));
		char *reg = find_reg();		// new reg
		memcpy(result, reg, strlen(reg));
		add_var_reg(tmp, reg);
		add_reg_var(reg, var);
	}
	return result;
}

char *find_reg(){
	char *result = (char *)malloc(ID_MAX_LEN);
	memset(result, 0, ID_MAX_LEN);
	int i;
	for (i = 0; i < 20; i ++){
		if (regs[i].list == NULL){
			memcpy(result, regs[i].name, strlen(regs[i].name));
			regs[i].list = (var_list_node *)malloc(sizeof(var_list_node));
			return result;
		}
	}
	return "regnotfind";
}

char *get_addr(char *var){
	if (var[0] == '&') var = var+1;
	if (!is_var_exist(var))
		add_var(var, 4);

	var_store *trace = vars;
	while (trace != NULL){
		int cmplen = strlen(var);
		if (var[cmplen-1] == '\n') cmplen--;
		if (memcmp(var, trace->name, cmplen) == 0) break;
		trace = trace->next;
	}
	char *regt = find_reg();
	exe_code *tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "\taddi %s, $fp, %d\n", regt, trace->fpoffset);
/*	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, "\tla %s, %s\n", regt, regt);*/
	return regt;
}

void add_var_reg(var_store *var, char *reg){
	addr_list_node *node = (addr_list_node *)malloc(sizeof(addr_list_node));
	memset(node, 0, sizeof(addr_list_node));
	memcpy(node->addr, reg, strlen(reg));
	node->next = var->list;
	var->list = node;
}
void add_reg_var(char *reg, char *var){
	int i;
	for (i = 0; i < 20; i ++){
		if (memcmp(regs[i].name, reg, strlen(reg)) == 0)
			break;
	}
	if (i < 20){
		var_list_node *tmp = (var_list_node *)malloc(sizeof(var_list_node));
		memset(tmp, 0, sizeof(var_list_node));
		memcpy(tmp->name, var, strlen(var));
		tmp->next = regs[i].list;
		regs[i].list = tmp;
	}
	else {
		printf("error reg %s\n", reg);
	}
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

void init_all_var(){
	Midcode *trace = codes;
	while (trace != NULL){
		int i;
		if (isLABEL(trace)){
		}
		else if (isGOTO(trace)){
		}
		else if (isIF(trace)){
		}
		else if (isRETURN(trace)){
		}
		else if (isFUNCTION(trace)){
		}
		else if (isASSIGN(trace)){
			char *var1 = get_word(trace->sentence, 0);
			if (var1[0] != '*' && var1[0] != '#' && var1[0] != '&'){
				if (!is_var_exist(var1)){
					exe_code *tmp = new_ecode(NULL);
					sprintf(tmp->sentence, "%s: .word 0\n", var1);
					add_var(var1, 4);
				}
			}
		}
		else if (isPLUS(trace)){
			char *var1 = get_word(trace->sentence, 0);
			if (var1[0] != '*' && var1[0] != '#' && var1[0] != '&'){
				if (!is_var_exist(var1)){
					exe_code *tmp = new_ecode(NULL);
					sprintf(tmp->sentence, "%s: .word 0\n", var1);
					add_var(var1, 4);
				}
			}
		}
		else if (isMINUS(trace)){
			char *var1 = get_word(trace->sentence, 0);
			if (var1[0] != '*' && var1[0] != '#' && var1[0] != '&'){
				if (!is_var_exist(var1)){
					exe_code *tmp = new_ecode(NULL);
					sprintf(tmp->sentence, "%s: .word 0\n", var1);
					add_var(var1, 4);
				}
			}
		}
		else if (isSTAR(trace)){
			char *var1 = get_word(trace->sentence, 0);
			if (var1[0] != '*' && var1[0] != '#' && var1[0] != '&'){
				if (!is_var_exist(var1)){
					exe_code *tmp = new_ecode(NULL);
					sprintf(tmp->sentence, "%s: .word 0\n", var1);
					add_var(var1, 4);
				}
			}
		}
		else if (isDIV(trace)){
			char *var1 = get_word(trace->sentence, 0);
			if (var1[0] != '*' && var1[0] != '#' && var1[0] != '&'){
				if (!is_var_exist(var1)){
					exe_code *tmp = new_ecode(NULL);
					sprintf(tmp->sentence, "%s: .word 0\n", var1);
					add_var(var1, 4);
				}
			}
		}
		else if (isDEC(trace)){
			char var1[ID_MAX_LEN];
			int size;
			memset(var1, 0, ID_MAX_LEN);
			sscanf(trace->sentence, "DEC %s %d", var1, &size);
			exe_code *tmp = new_ecode(NULL);
			sprintf(tmp->sentence, "%s: .space %d\n", var1, size);
			add_var(var1, 4);
		}
		else if (isARG(trace)){
		}
		else if (isCALL(trace)){
		}
		else if (isPARAM(trace)){
		}
		else if (isREAD(trace)){
			char *var1 = get_word(trace->sentence, 1);
			if (var1[strlen(var1)-1] == '\n') var1[strlen(var1)-1] = 0;
			if (var1[0] != '*' && var1[0] != '#' && var1[0] != '&' && memcmp(var1, "READ", 4) != 0){
				if (!is_var_exist(var1)){
					exe_code *tmp = new_ecode(NULL);
					sprintf(tmp->sentence, "%s: .word 0\n", var1);
					add_var(var1, 4);
				}
			}
		}
		else if (isWRITE(trace)){
		}
		else {
			printf("error classify\n");
		}
		trace = trace->next;
	}
}
