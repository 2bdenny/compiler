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

char *get_word(char *sentence, int n){
	char *result = NULL;
	int space_count = 0;

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
		if (trace == NULL) {
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
					char *reg1 = get_reg(var1+1);
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "sw %s, 0(%s)\n", var2+1, reg1);
				}
				else if (var2[0] == '*'){	// *x = *y
					char *reg1 = get_reg(var1+1);
					char *reg2 = get_reg(var2+1);
					char *regt = find_reg();
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "lw %s, 0(%s)\n", regt, reg2);
					tmp = new_ecode(block);
					sprintf(tmp->sentence, "sw %s, 0(%s)\n", regt, reg1);
				}
				else if (var2[0] == '&'){	// *x = &y
					char *addr2 = get_addr(var2+1);
					char *reg1 = get_reg(var1+1);
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "sw %s, 0(%s)\n", addr2, reg1);
				}
				else {	// *x = y
					char *reg1 = get_reg(var1+1);
					char *reg2 = get_reg(var2);
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "sw %s, 0(%s)\n", reg2, reg1);
				}
			}
			else{
				if (var2[0] == '#'){	// x = #1
					char *reg1 = get_reg(var1);
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "li %s, %s\n", reg1, var2+1);
				}
				else if (var2[0] == '*'){	// x = *y
					char *reg1 = get_reg(var1);
					char *reg2 = get_reg(var2+1);
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "lw %s, 0(%s)\n", reg1, reg2);
				}
				else if (var[2] == '&'){	// x = &y
					char *addr2 = get_addr(var2+1);
					char *reg1 = get_reg(var1);
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "li %s, %s\n", reg1, addr2);
				}
				else {	// x = y
					char *reg1 = get_reg(var1);
					char *reg2 = get_reg(var2);
					exe_code *tmp = new_ecode(block);
					sprintf(tmp->sentence, "move %s, %s\n", reg1, reg2);
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
						char *reg1 = get_reg(var1+1);
						char *regt = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "addi %s, %s, %s\n", regt, var2+1, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "sw %s, 0(%s)\n", regt, reg1);
					}
					else {	// *x = #1 + y
						char *reg1 = get_reg(var1+1);
						char *reg2 = get_reg(var2);
						char *regt = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "addi %s, %s, %s\n", regt, reg2+1, var3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "sw %s, 0(%s)\n", regt, reg1);
					}
				}
				else {
					if (var3[0] == '#'){	// *x = y + #1
						char *reg1 = get_reg(var1);
						char *regt = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "addi %s, %s, %s\n", regt, var2, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "sw %s, 0(%s)\n", regt, reg1);
					}
					else {	// *x = y + z
						char *reg1 = get_reg(var1);
						char *reg2 = get_reg(var2);
						char *reg3 = get_reg(var3);
						char *regt = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "add %s, %s, %s\n", regt, reg2, var3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "sw %s, 0(%s)\n", regt, reg1);
					}
				}
			}
			else {
				if (var2[0] == '*'){
					if (var3[0] == '*'){	// x = *y + *2
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						char *reg1 = get_reg(var1);
						char *reg2 = get_reg(var2+1);
						char *reg3 = get_reg(var3+1);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, 0(%s)\n", regt2, reg2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, 0(%s)\n", regt3, reg3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "add %s, %s, %s\n", reg1, regt2, regt3);
					}
					else if (var3[0] == '#'){	// x = *y + #2
						char *regt2 = find_reg();
						char *reg1 = get_reg(var1);
						char *reg2 = get_reg(var2+1);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, 0(%s)\n", regt2, reg2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "addi %s, %s, %s\n", reg1, regt2, var3+1);
					}
					else {	// x = *y + z
						char *regt2 = find_reg();
						char *reg1 = get_reg(var1);
						char *reg2 = get_reg(var2+1);
						char *reg3 = get_reg(var3);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, 0(%s)\n", regt2, reg2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "add %s, %s, %s\n", reg1, regt2, reg3);
					}
				}
				else if (var2[0] == '#'){
					if (var3[0] == '*'){	// x = #2 + *y
						char *regt3 = find_reg();
						char *reg1 = get_reg(var1);
						char *reg3 = ger_reg(var3+1);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, 0(%s)\n", regt3, reg3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "addi %s, %s, %s\n", reg1, regt3, var2+1);
					}
					else if (var3[0] == '#'){	// x = #1 + #2
						char *reg1 = get_reg(var1);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "addi %s, %s, %s\n", reg1, var2+1, var3+1);
					}
					else {	// x = #1 + y
						char *reg1 = get_reg(var1);
						char *reg3 = get_reg(var3);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "addi %s, %s, %s\n", reg1, reg3, var2+1);
					}
				}
				else {
					if (var3[0] == '*'){	// x = y + *z
						char *regt3 = find_reg();
						char *reg1 = get_reg(var1);
						char *reg2 = get_reg(var2);
						char *reg3 = get_reg(var3+1);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, 0(%s)\n", regt3, reg3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "add %s, %s, %s\n", reg1, reg2, regt3);
					}
					else if (var3[0] == '#'){	// x = y + #2
						char *reg1 = get_reg(var1);
						char *reg2 = ger_reg(var2);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "addi %s, %s, %s\n", reg1, reg2, reg3+1);
					}
					else {	// x = y + z
						char *reg1 = get_reg(var1);
						char *reg2 = get_reg(var2);
						char *reg3 = get_reg(var3);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "add %s, %s, %s\n", reg1, reg2, reg3);
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
						char *reg1 = get_reg(var1+1);
						char *regt = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "addi %s, %s, -%s\n", regt, var2+1, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "sw %s, 0(%s)\n", regt, reg1);
					}
					else {	// *x = #1 - y
						char *reg1 = get_reg(var1+1);
						char *reg3 = get_reg(var3);
						char *regt = find_reg();
						char *regt2 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "li %s, %s", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "sub %s, %s, %s\n", regt, regt2, reg3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "sw %s, 0(%s)\n", regt, reg1);
					}
				}
				else {
					if (var3[0] == '#'){	// *x = y - #1
						char *reg1 = get_reg(var1);
						char *regt = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "addi %s, %s, -%s\n", regt, var2, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "sw %s, 0(%s)\n", regt, reg1);
					}
					else {	// *x = y - z
						char *reg1 = get_reg(var1);
						char *reg2 = get_reg(var2);
						char *reg3 = get_reg(var3);
						char *regt = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "sub %s, %s, %s\n", regt, reg2, var3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "sw %s, 0(%s)\n", regt, reg1);
					}
				}
			}
			else {
				if (var2[0] == '*'){
					if (var3[0] == '*'){	// x = *y - *z
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						char *reg1 = get_reg(var1);
						char *reg2 = get_reg(var2+1);
						char *reg3 = get_reg(var3+1);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, 0(%s)\n", regt2, reg2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, 0(%s)\n", regt3, reg3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "sub %s, %s, %s\n", reg1, regt2, regt3);
					}
					else if (var3[0] == '#'){	// x = *y - #2
						char *regt2 = find_reg();
						char *reg1 = get_reg(var1);
						char *reg2 = get_reg(var2+1);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, 0(%s)\n", regt2, reg2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "addi %s, %s, -%s\n", reg1, regt2, var3+1);
					}
					else {	// x = *y - z
						char *regt2 = find_reg();
						char *reg1 = get_reg(var1);
						char *reg2 = get_reg(var2+1);
						char *reg3 = get_reg(var3);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, 0(%s)\n", regt2, reg2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "sub %s, %s, %s\n", reg1, regt2, reg3);
					}
				}
				else if (var2[0] == '#'){
					if (var3[0] == '*'){	// x = #2 - *y
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						char *reg1 = get_reg(var1);
						char *reg3 = ger_reg(var3+1);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, 0(%s)\n", regt3, reg3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "li %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "sub %s, %s, %s\n", reg1, regt2, regt3);
					}
					else if (var3[0] == '#'){	// x = #1 - #2
						char *reg1 = get_reg(var1);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "addi %s, %s, -%s\n", reg1, var2+1, var3+1);
					}
					else {	// x = #1 - y
						char *regt2 = find_reg();
						char *reg1 = get_reg(var1);
						char *reg3 = get_reg(var3);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "li %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "sub %s, %s, %s\n", reg1, regt2, reg3);
					}
				}
				else {
					if (var3[0] == '*'){	// x = y - *z
						char *regt3 = find_reg();
						char *reg1 = get_reg(var1);
						char *reg2 = get_reg(var2);
						char *reg3 = get_reg(var3+1);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, 0(%s)\n", regt3, reg3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "sub %s, %s, %s\n", reg1, reg2, regt3);
					}
					else if (var3[0] == '#'){	// x = y - #2
						char *reg1 = get_reg(var1);
						char *reg2 = ger_reg(var2);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "addi %s, %s, -%s\n", reg1, reg2, reg3+1);
					}
					else {	// x = y - z
						char *reg1 = get_reg(var1);
						char *reg2 = get_reg(var2);
						char *reg3 = get_reg(var3);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "sub %s, %s, %s\n", reg1, reg2, reg3);
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
					if (var3[0] == '#'){	// *x = #1 * #2
						char *reg1 = get_reg(var1+1);
						char *regt = find_reg();
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "li %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "li %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "mul %s, %s, %s\n", regt, regt2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "sw %s, 0(%s)\n", regt, reg1);
					}
					else {	// *x = #1 * y
						char *regt = find_reg();
						char *regt2 = find_reg();
						char *reg1 = get_reg(var1+1);
						char *reg3 = get_reg(var3);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "li %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "mul %s, %s, %s\n", regt, regt2, reg3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "sw %s, 0(%s)\n", regt, reg1);
					}
				}
				else {
					if (var3[0] == '#'){	// *x = y * #1
						char *regt = find_reg();
						char *regt3 = find_reg();
						char *reg1 = get_reg(var1);
						char *reg2 = get_ret(var2);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "li %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "mul %s, %s, %s\n", regt, reg2, regt3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "sw %s, 0(%s)\n", regt, reg1);
					}
					else {	// *x = y * z
						char *reg1 = get_reg(var1);
						char *reg2 = get_reg(var2);
						char *reg3 = get_reg(var3);
						char *regt = find_reg();
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "mul %s, %s, %s\n", regt, reg2, reg3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "sw %s, 0(%s)\n", regt, reg1);
					}
				}
			}
			else {
				if (var2[0] == '*'){
					if (var3[0] == '*'){	// x = *y * *2
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						char *reg1 = get_reg(var1);
						char *reg2 = get_reg(var2+1);
						char *reg3 = get_reg(var3+1);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, 0(%s)\n", regt2, reg2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, 0(%s)\n", regt3, reg3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "mul %s, %s, %s\n", reg1, regt2, regt3);
					}
					else if (var3[0] == '#'){	// x = *y * #2
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						char *reg1 = get_reg(var1);
						char *reg2 = get_reg(var2+1);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, 0(%s)\n", regt2, reg2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "li %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "mul %s, %s, %s\n", reg1, regt2, regt3);
					}
					else {	// x = *y + z
						char *regt2 = find_reg();
						char *reg1 = get_reg(var1);
						char *reg2 = get_reg(var2+1);
						char *reg3 = get_reg(var3);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, 0(%s)\n", regt2, reg2);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "mul %s, %s, %s\n", reg1, regt2, reg3);
					}
				}
				else if (var2[0] == '#'){
					if (var3[0] == '*'){	// x = #2 * *y
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						char *reg1 = get_reg(var1);
						char *reg3 = ger_reg(var3+1);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "li %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, 0(%s)\n", regt3, reg3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "mul %s, %s, %s\n", reg1, regt2, regt3);
					}
					else if (var3[0] == '#'){	// x = #1 * #2
						char *regt2 = find_reg();
						char *regt3 = find_reg();
						char *reg1 = get_reg(var1);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "li %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "li %s, %s\n", regt3, var3+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "mul %s, %s, %s\n", reg1, regt2, regt3);
					}
					else {	// x = #1 * y
						char *regt2 = find_reg();
						char *reg1 = get_reg(var1);
						char *reg3 = get_reg(var3);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "li %s, %s\n", regt2, var2+1);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "mul %s, %s, %s\n", reg1, regt2, reg3);
					}
				}
				else {
					if (var3[0] == '*'){	// x = y * *z
						char *regt3 = find_reg();
						char *reg1 = get_reg(var1);
						char *reg2 = get_reg(var2);
						char *reg3 = get_reg(var3+1);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "lw %s, 0(%s)\n", regt3, reg3);
						tmp = new_ecode(block);
						sprintf(tmp->sentence, "mul %s, %s, %s\n", reg1, reg2, regt3);
					}
					else if (var3[0] == '#'){	// x = y * #2
						char *regt3 = find_reg();
						char *reg1 = get_reg(var1);
						char *reg2 = ger_reg(var2);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "li %s, %s\n", regt3, var3+1);
						sprintf(tmp->sentence, "mul %s, %s, %s\n", reg1, reg2, regt3);
					}
					else {	// x = y * z
						char *reg1 = get_reg(var1);
						char *reg2 = get_reg(var2);
						char *reg3 = get_reg(var3);
						exe_code *tmp = new_ecode(block);
						sprintf(tmp->sentence, "mul %s, %s, %s\n", reg1, reg2, reg3);
					}
				}
			}
		}
		else if (isDIV(trace)){}
		else if (isGOTO(trace)){
			exe_code *tmp = new_ecode(block);
			sprintf(tmp->sentence, "j %s\n", get_word(trace->sentence, 1));
		}
		else if (isIF(trace)){}
		else if (isRETURN(trace)){}
		else if (isDEC(trace)){}
		else if (isARG(trace)){}
		else if (isCALL(trace)){}
		else if (isPARAM(trace)){}
		else if (isREAD(trace)){}
		else if (isWRITE(trace)){}
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
	sprintf(tmp->sentence, "_ret: .asciiz \"\n\"\n");
	tmp = new_ecode(NULL);
	sprintf(tmp->sentence, ".globl main\n");
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
}
