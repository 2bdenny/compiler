#include <stdio.h>
#include "GrammarTree.h"
#include "syntax.tab.h"
//#include "lex.yy.c"

//#define YYDEBUG 1
//extern FILE *yyin;
extern Tree *forest;
extern Item *table;
int main(int argc, char *argv[]){
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
		displayMidcode();
		int i = strlen(argv[1]);
		for (i--; i >= 0; i --)
			if (argv[1][i] == '.') break;
		memset(argv[1]+i+1, 0, strlen(argv[1])-i-1);
		sprintf(argv[1]+i+1, "%s", "ir");
		storeMidcode(argv[1]);

//		analyse(forest->tree);
		destroyForest();
	}
	else printf("Usage: ./paser *.cmm\n");
	return 0;
}
