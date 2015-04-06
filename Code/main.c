#include <stdio.h>
//extern FILE *yyin;
int main(int argc, char *argv[]){
	if (argc > 1) {
		FILE *f;
		if (!(f = fopen(argv[1], "r"))){
			perror(argv[1]);
			return 1;
		}
		yyrestart(f);
		yyparse();
		fclose(f);
	}
	else printf("Usage: ./scanner *.chm\n");
	return 0;
}
