#include <stdio.h>
#include "GrammarTree.h"
//#define YYDEBUG 1
//extern FILE *yyin;
//extern Tree *forest;
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
	}
	else printf("Usage: ./scanner *.chm\n");
	return 0;
}
