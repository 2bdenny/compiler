#include "stack.h"

int main(){
	char x[][10] = {"a", "aa", "aaa", "aaaa", "aaaaa", "ab", "abb", "abbb", "abbbb"};
	int i;
	for (i = 0; i < 65555; i ++){
		push(x[i%10]);
	}
//	display();
/*	for (i = 0; i < 9; i ++){
		printf("Stack size = %d\n", size());
		pop();
		pop();
		display();
	}*/
	return 0;
}
