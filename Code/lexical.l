%option yylineno
%{
	#define YYSTYPE char*
	#include "syntax.tab.h"
	#include "GrammarTree.h"
	#include "SDT.h"
	int yycolumn = 1;
	#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;yylloc.first_column = yycolumn; yylloc.last_column = yycolumn + yyleng-1;yycolumn += yyleng;
void plantINT(int radix);
void plantFLOAT();
void plantID();
void plantRELOP();
void plantKEY(char *name);
void plantSYMBOL(char *syb);
int oct2int(char *oct, int leng);
int hex2int(char *hex, int leng);
%}
/* regex */
nl	[\r]?\n
ws	{nl}+|[ \t\n]+
letter	[A-Za-z_]
digit8	[0-7]
digit10	[0-9]
digit16	[0-9A-Fa-f]
int8	-?0(0|[1-7]{digit8}*)
int10	-?(0|[1-9]{digit10}*)
int16	-?0[xX](0|[1-9A-Fa-f]{digit16}*)
err16	-?0[xX][0-9A-Za-z]*
err8	-?0[0-9A-WYZa-wyz][0-9A-Za-z]*
id	{letter}({letter}|{digit10})*
float	{int10}(\.{digit10}+)?([Ee]{int10})?

%%
{ws}	{/*no action and no return*/}
if	{ plantKEY("IF"); return IF;}
else	{ plantKEY("ELSE"); return ELSE;}
int	{ plantKEY("int"); return TYPE;}
float	{ plantKEY("float"); return TYPE;}
struct 	{ plantKEY("STRUCT"); return STRUCT;}
return	{ plantKEY("RETURN"); return RETURN;}
while	{ plantKEY("WHILE"); return WHILE;}
{id}	{ plantID(); return ID;}
{int8}	{ plantINT(8); return INT;}
{err8}	{ printf("Error type A at Line %d: Error OCT \"%s\"\n", yylineno, yytext); plantINT(0); meetError(); return INT;}
{int10}	{ plantINT(10); return INT;}
{int16}	{ plantINT(16); return INT;}
{err16}	{ printf("Error type A at Line %d: Error HEX \"%s\"\n", yylineno, yytext); plantINT(0); meetError(); return INT;}
{float}	{ plantFLOAT(); return FLOAT;}
";"	{ plantSYMBOL("SEMI"); return SEMI;}
","	{ plantSYMBOL("COMMA"); return COMMA;}
"="	{ plantSYMBOL("ASSIGNOP"); return ASSIGNOP;}
"+"	{ plantSYMBOL("PLUS"); return PLUS;}
"-"	{ plantSYMBOL("MINUS"); return MINUS;}
"*"	{ plantSYMBOL("STAR"); return STAR;}
"/"	{ plantSYMBOL("DIV"); return DIV;}
"&&"	{ plantSYMBOL("AND"); return AND;}
"||"	{ plantSYMBOL("OR"); return OR;}
"!"	{ plantSYMBOL("NOT"); return NOT;}
"."	{ plantSYMBOL("DOT"); return DOT;}
"("	{ plantSYMBOL("LP"); return LP;}
")"	{ plantSYMBOL("RP"); return RP;}
"["	{ plantSYMBOL("LB"); return LB;}
"]"	{ plantSYMBOL("RB"); return RB;}
"{"	{ plantSYMBOL("LC"); return LC;}
"}"	{ plantSYMBOL("RC"); return RC;}
"<"	{ plantRELOP(); return RELOP;}
"<="	{ plantRELOP(); return RELOP;}
"=="	{ plantRELOP(); return RELOP;}
"<>"	{ plantRELOP(); return RELOP;}
">"	{ plantRELOP(); return RELOP;}
">="	{ plantRELOP(); return RELOP;}
.	{ printf("Error type A at Line %d: Mysterious Character \"%s\"\n", yylineno, yytext); meetError();}

%%
int yywrap(){
	return 1;
}
void plantINT(int radix){
	Value vl;
	vl.val_double = 0.0;
	vl.val_float = 0.0;
	memset(vl.val_name, 0, 32);
	int valno = 1;
	switch(radix){
		case 0: {memcpy(vl.val_name, yytext, yyleng); valno = 3;} break;
		case 8: vl.val_int = oct2int(yytext, yyleng); break;
		case 10: vl.val_int = atoi(yytext); break;
		case 16: vl.val_int = hex2int(yytext, yyleng); break;
		default: vl.val_int = 0; break;
	}
	Leaf *tmp = makeLeaf(yylineno, valno, 1, "INT", vl);

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
int hex2int(char *hex, int leng){
	if (leng < 3) return -1;
	else {
		int result = 0;
		int ex = 1;
		int i = leng-1;
		int limit;
		if (hex[0] == '-') limit = 2;
		else limit = 1;
		while (i > limit) {
			if (hex[i] >= '0' && hex[i] <= '9'){
				result += (hex[i]-'0')*ex;
			}
			else if (hex[i] >= 'a' && hex[i] <= 'f'){
				result += (hex[i]-'a'+10)*ex;
			}
			else if (hex[i] >= 'A' && hex[i] <= 'F'){
				result += (hex[i]-'A'+10)*ex;
			}
			else return -1;
			i --;
			ex *= 16;
		}
		if (limit == 1) return result;
		else return -result;
	}
}

int oct2int(char *oct, int leng){
	if (leng < 2) return -1;
	else {
		int result = 0;
		int ex = 1;
		int i = leng-1;
		int limit;
		if (oct[0] == '-') limit = 1;
		else limit = 0;
		while (i > limit){
			if (oct[i] >= '0' && oct[i] <= '7') {
				result += (oct[i] - '0')*ex;
			}
			else return -1;
			i --;
			ex *= 8;
		}
		if (limit == 0) return result;
		else return -result;
	}
}
