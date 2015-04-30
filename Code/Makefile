all: lex.yy.c syntax.tab.h syntax.tab.c syntax.output ../parser
.PHONY: all

lex.yy.c: lexical.l
	flex lexical.l
syntax.tab.h syntax.tab.c syntax.output: syntax.y
	bison -d -t -v syntax.y
../parser: main.c GrammarTree.c syntax.tab.c
	cc main.c GrammarTree.c syntax.tab.c -o ../parser -g

clean:
	rm *.yy.c *.tab.c *.tab.h *.output ../parser
