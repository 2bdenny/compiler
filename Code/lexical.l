%option yylineno
%{
	#define YYSTYPE char*
	#include "syntax.tab.h"
	#include "GrammarTree.h"
	int yycolumn = 1;
	#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;yylloc.first_column = yycolumn; yylloc.last_column = yycolumn + yyleng-1;yycolumn += yyleng;
void plantINT(int radix);
void plantFLOAT();
void plantID();
void plantRELOP();
void plantKEY(char *name);
void plantSYMBOL(char *syb);
%}
/* regex */
nl	\n
ws	[ \t\n]+
letter	[A-Za-z_]
digit8	[0-7]
digit10	[0-9]
digit16	[0-9A-Fa-f]
int8	-?0(0|[1-7]{digit8}*)
int10	-?(0|[1-9]{digit10}*)
int16	-?0x(0|[1-9A-Fa-f]{digit16}*)
id	{letter}({letter}|{digit10})*
float	{int10}(\.{digit10}+)?([Ee]{int10})?

%%
{ws}	{/*no action and no return*/}
if	{printf("IF\n"); plantKEY("IF"); return IF;}
else	{printf("ELSE\n"); plantKEY("ELSE"); return ELSE;}
int	{printf("TYPE\n"); plantKEY("int"); return TYPE;}
float	{printf("TYPE\n"); plantKEY("float"); return TYPE;}
struct 	{printf("STRUCT\n"); plantKEY("STRUCT"); return STRUCT;}
return	{printf("RETURN\n"); plantKEY("RETURN"); return RETURN;}
while	{printf("WHILE\n"); plantKEY("WHILE"); return WHILE;}
{id}	{printf("ID\n"); plantID(); return ID;}
{int8}	{printf("INT\n"); plantINT(8); return INT;}
{int10}	{printf("INT\n"); plantINT(10); return INT;}
{int16}	{printf("INT\n"); plantINT(16); return INT;}
{float}	{printf("FLOAT\n"); plantFLOAT(); return FLOAT;}
";"	{printf("SEMI\n"); plantSYMBOL("SEMI"); return SEMI;}
","	{printf("COMMA\n"); plantSYMBOL("COMMA"); return COMMA;}
"="	{printf("ASSIGNOP\n"); plantSYMBOL("ASSIGNOP"); return ASSIGNOP;}
"+"	{printf("PLUS\n"); plantSYMBOL("PLUS"); return PLUS;}
"-"	{printf("MINUS\n"); plantSYMBOL("MINUS"); return MINUS;}
"*"	{printf("STAR\n"); plantSYMBOL("STAR"); return STAR;}
"/"	{printf("DIV\n"); plantSYMBOL("DIV"); return DIV;}
"&&"	{printf("AND\n"); plantSYMBOL("AND"); return AND;}
"||"	{printf("OR\n"); plantSYMBOL("OR"); return OR;}
"!"	{printf("NOT\n"); plantSYMBOL("NOT"); return NOT;}
"."	{printf("DOT\n"); plantSYMBOL("DOT"); return DOT;}
"("	{printf("LP\n"); plantSYMBOL("LP"); return LP;}
")"	{printf("RP\n"); plantSYMBOL("RP"); return RP;}
"["	{printf("LB\n"); plantSYMBOL("LB"); return LB;}
"]"	{printf("RB\n"); plantSYMBOL("RB"); return RB;}
"{"	{printf("LC\n"); plantSYMBOL("LC"); return LC;}
"}"	{printf("RC\n"); plantSYMBOL("RC"); return RC;}
"<"	{printf("RELOP\n"); plantRELOP(); return RELOP;}
"<="	{printf("RELOP\n"); plantRELOP(); return RELOP;}
"=="	{printf("RELOP\n"); plantRELOP(); return RELOP;}
"<>"	{printf("RELOP\n"); plantRELOP(); return RELOP;}
">"	{printf("RELOP\n"); plantRELOP(); return RELOP;}
">="	{printf("RELOP\n"); plantRELOP(); return RELOP;}
.	{printf("Error type A at Line %d: Mysterious Character \"%s\"\n", yylineno, yytext);}

%%
int yywrap(){
	return 1;
}
void plantINT(int radix){
	Value vl;
	vl.val_double = 0.0;
	vl.val_float = 0.0;
	memset(vl.val_name, 0, 32);
	switch(radix){
		case 8: vl.val_int = atoi(yytext+1);
		case 10: vl.val_int = atoi(yytext);
		case 16: vl.val_int = atoi(yytext+2);
		default: vl.val_int = 0;
	}
	Leaf *tmp = makeLeaf(yylineno, 1, 1, "INT", vl);

	yylval = (char *)tmp;
	addTree(tmp);
}
void plantFLOAT(){
	Value vl;
	vl.val_int = 0;
	vl.val_double = atof(yytext);
	vl.val_float = atof(yytext);
	memset(vl.val_name, 0, 32);

	Leaf *tmp = makeLeaf(yylineno, 0, 1, "FLOAT", vl);
	yylval = (char *)tmp;
	addTree(tmp);
}
void plantID(){
	Value vl;
	vl.val_int = 0;
	vl.val_double = 0.0;
	vl.val_float = 0.0;
	memset(vl.val_name, 0, 32);
	memcpy(vl.val_name, yytext, yyleng);

	Leaf *tmp = makeLeaf(yylineno, 3, 1, "ID", vl);
	yylval = (char *)tmp;
	addTree(tmp);
}
void plantKEY(char *name) {
	Value vl;
	vl.val_int = 0;
	vl.val_double = 0.0;
	vl.val_float = 0.0;
	memset(vl.val_name, 0, 32);

	Leaf *tmp = NULL;
	makeLeaf(yylineno, 3, 1, "ID", vl);
	switch(name[0]){
		case 'i': 
		case 'f':{memcpy(vl.val_name, name, strlen(name)); tmp = makeLeaf(yylineno, 3, 1, "TYPE", vl);} break;
		default: {tmp = makeLeaf(yylineno, -1, 1, name, vl);} break;
	}
	yylval = (char *)tmp;
	addTree(tmp);
}
void plantSYMBOL(char *syb){
	Value vl;
	vl.val_int = 0;
	vl.val_double = 0.0;
	vl.val_float = 0.0;
	memset(vl.val_name, 0, 32);

	Leaf *tmp = makeLeaf(yylineno, -1, 1, syb, vl);
	yylval = (char *)tmp;
	addTree(tmp);
}
void plantRELOP(){
	Value vl;
	vl.val_int = 0;
	vl.val_double = 0.0;
	vl.val_float = 0.0;
	memset(vl.val_name, 0, 32);
	memcpy(vl.val_name, yytext, yyleng);

	Leaf *tmp = makeLeaf(yylineno, 3, 1, "RELOP", vl);
	yylval = (char *)tmp;
	addTree(tmp);
}
