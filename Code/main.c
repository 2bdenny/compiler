#include <stdio.h>
#include "GrammarTree.h"
#include "generator.h"
#include "syntax.tab.h"
//#include "lex.yy.c"

//#define YYDEBUG 1
//extern FILE *yyin;
extern Tree *forest;
extern Item *table;
int main(int argc, char *argv[]){
	init_var();
//	forest = NULL;
	if (argc > 1) {
		FILE *f;
		if (!(f = fopen(argv[1], "r"))){
			perror(argv[1]);
			return 1;
		}
		initTable();
		yyrestart(f);
//		printf("yyrestart ok\n");
//		yydebug = 1;
		yyparse();
		fclose(f);
//		display(forest->tree, 0);
//		analyse(forest->tree);
//		displayTable(table);
//		displayMidcode();
		int i = strlen(argv[1]);
		for (i--; i >= 0; i --)
			if (argv[1][i] == '.') break;
		memset(argv[1]+i+1, 0, strlen(argv[1])-i-1);
		sprintf(argv[1]+i+1, "%s", "ir");
		storeMidcode(argv[1]);

		// 划分基本块
		part_base_block();
//		display_base_block();

		/*char *x = "hello, every one, i'm monkey coder, the weather of today is good\n";
		int thex = 0;
		for (; thex < 20; thex ++){
			printf("%d: %s\n", thex, get_word(x, thex));
		}*/
//		analyse(forest->tree);

		// 把机器代码保存到.s文件里

   		init_ecode();
		generate();
   		memset(argv[1]+i+1, 0, strlen(argv[1])-i-1);
		sprintf(argv[1]+i+1, "%s", "s");
		store_exe_code(argv[1]);

		destroyForest();
	}
	else printf("Usage: ./paser *.cmm\n");
	return 0;
}
