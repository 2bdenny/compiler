#include <stdio.h>
#include "GrammarTree.h"
#include "syntax.tab.h"
#include "syntaxAnalyse.h"
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
		yyrestart(f);
//		yydebug = 1;
		yyparse();
		fclose(f);
//		display(forest->tree, 0);
		analyse(forest->tree);
		printf("\n\n\n\n\n\n");
		displayTable(table);

		analyseGrammer(forest->tree);
		destroyForest();
	}
	else printf("Usage: ./paser *.cmm\n");
	return 0;
}
